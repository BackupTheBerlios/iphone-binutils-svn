/* ----------------------------------------------------------------------------
 *   weasel - a minimalist debugger for Mac OS X           v0.01 - 07/23/2007
 *   Copyright (c) 2007 Patrick Walton but freely redistributable under the
 *   terms specified in the COPYING file.
 * ------------------------------------------------------------------------- */

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <mach/mach_init.h>
#include <mach/mach_traps.h>
#include <mach/thread_act.h>
#include <mach/vm_map.h>
#include <mach-o/loader.h>
#include <mach-o/nlist.h>
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "weasel.h"

struct weasel_symbol {
    unsigned int addr;
    union {
        char *str;
        unsigned int index;
    } name;
};

/* TODO: make a sys header for this guy */
kern_return_t task_threads(task_t target_task, thread_array_t *act_list,
    mach_msg_type_number_t *act_listCnt);

int inferior_in_ptrace = 1;
task_t inferior_task;
thread_array_t inferior_threads = NULL;
unsigned int inferior_thread_count = 0;
unsigned int last_disassembly_point = 0;
struct weasel_symbol **symbol_table = NULL;
unsigned int symbol_table_size = 0;

pid_t start_inferior(int argc, char **argv)
{
    char **child_args;
    int status;
    pid_t kid;

    if ((kid = fork())) {
        wait(&status);
        if (WIFSTOPPED(status)) {
            fprintf(stderr, "Process with PID %d started...\n", (int)kid);
            return kid;
        }

        exit(0);
    }

    fprintf(stderr, "argc=%d\n", argc);
    child_args = (char **)malloc(sizeof(char *) * (argc + 1));
    memcpy(child_args, argv, sizeof(char *) * argc);
    child_args[argc] = NULL;

    ptrace(PT_TRACE_ME, 0, 0, 0);
    execvp(argv[0], child_args);

    fprintf(stderr, "Failed to start inferior.\n");
    exit(1);

    return 0;   /* should never get here */
}

void attach_to_process(pid_t inferior)
{
    kern_return_t err;

    if ((err = task_for_pid(mach_task_self(), (int)inferior, &inferior_task) !=
        KERN_SUCCESS)) {
        fprintf(stderr, "Failed to get task for pid %d: %d.\n", (int)inferior,
            (int)err);
        exit(1);
    }

    if (task_threads(inferior_task, &inferior_threads, &inferior_thread_count)
        != KERN_SUCCESS) {
        fprintf(stderr, "Failed to get list of task's threads.\n");
        exit(1);
    }

}

void get_inferior_status(unsigned int *regs)
{
    kern_return_t err;
    unsigned int gp_count;
    unsigned int i;

    for (i = 0; i < inferior_thread_count; i++) {
        gp_count = 17;     
        if ((err = thread_get_state(inferior_threads[i], 1, (thread_state_t)
            regs, &gp_count)) != KERN_SUCCESS) {
            fprintf(stderr, "Failed to get thread %d state (%d).\n", (int)i,
                (int)err);
        }
    }
}

void show_registers(unsigned int *regs)
{
    int i;

    for (i = 0; i < 16; i += 8) {
        fprintf(stderr, "R %8d %8d %8d %8d %8d %8d %8d %8d\n", i, i+1, i+2,
            i+3, i+4, i+5, i+6, i+7);
        fprintf(stderr, "= %08x %08x %08x %08x %08x %08x %08x %08x\n", regs[i],
            regs[i+1], regs[i+2], regs[i+3], regs[i+4], regs[i+5], regs[i+6],
            regs[i+7]);
    }
}

void set_breakpoint(unsigned int addr)
{
    kern_return_t err;
    unsigned int inst;

    /* We know this is in the right endianness because it's our own! */
    inst = ((0xe12 << 20) | (0x7 << 4));

    err = vm_protect(inferior_task, addr, 4, 0, VM_PROT_READ | VM_PROT_WRITE |
        VM_PROT_EXECUTE); 
    if (err != KERN_SUCCESS) {
        fprintf(stderr, "Failed to unprotect memory: %d.\n", err);
        return;
    }

    err = vm_write(inferior_task, addr, (pointer_t)&inst,
        (mach_msg_type_number_t)4);
    if (err == KERN_SUCCESS)
        fprintf(stderr, "Breakpoint set at $%08x.\n", addr);
    else
        fprintf(stderr, "Failed to set breakpoint at $%08x: %d.\n", addr,
            (int)err);
}

void do_disassembly(unsigned int addr)
{
    char *assembler;
    unsigned int i, inst, size;
    kern_return_t err;

    err = vm_protect(inferior_task, addr, 4 * 23, 0, VM_PROT_READ |
        VM_PROT_WRITE | VM_PROT_EXECUTE); 
    if (err != KERN_SUCCESS) {
        fprintf(stderr, "Failed to unprotect memory: %d.\n", err);
        return;
    }

    for (i = addr; i < addr + 4 * 23; i += 4) {
        size = 4;
        err = vm_read_overwrite(inferior_task, i, 4, (pointer_t)&inst,
            &size);
        if (err != KERN_SUCCESS) {
            fprintf(stderr, "%08x ????????\t(inaccessible)\n", i);
            continue;
        }

        assembler = disassemble(inst, i); 
        fprintf(stderr, "%08x %08x\t%s\n", i, inst, assembler);
        free(assembler);
    }

    last_disassembly_point = i;
}

void nm()
{
    int i;

    for (i = 0; i < symbol_table_size; i++) {
        fprintf(stderr, "%08x %s\n", symbol_table[i]->addr,
            symbol_table[i]->name.str);
    }
}

void parse_and_handle_input(char *buf, pid_t inferior)
{
    int status;
    unsigned int addr;

    switch (buf[0]) {
        case 'b':
            if (sscanf(buf, "b %x", &addr) < 1) {
                fprintf(stderr, "usage: b addr-in-hex\n");
                break;
            }
            set_breakpoint(addr); 
            break;
        case 'c':
            if (inferior_in_ptrace)
                ptrace(PT_DETACH, inferior, 0, 0);
            /* task_resume(inferior_task); */
            wait(&status);
            break;
        case 'd':
            if (sscanf(buf, "d %x", &addr) < 1)
                do_disassembly(last_disassembly_point);
            else
                do_disassembly(addr);
            break;
        case 'n':
            nm();
            break;
        case 'q':
            ptrace(PT_KILL, inferior, 0, 0);
            wait(&status);
            exit(0);
            break;
        default:
            fprintf(stderr, "Available weasel commands:\n");
            fprintf(stderr,
                "    b set a breakpoint\n");
            fprintf(stderr,
                "    c continue execution\n");
            fprintf(stderr,
                "    d disassemble starting at address\n");
            fprintf(stderr,
                "      (if no args given, continues disassembly)\n");
            fprintf(stderr,
                "    n print the symbol table\n");
            fprintf(stderr,
                "    q quit weasel and inferior\n");
    }
}

void main_loop(char *inferior_name, pid_t inferior)
{
    char buf[256];
    unsigned int regs[17];

    while (1) {
        get_inferior_status(regs);
        fprintf(stderr, "[$%08x weasel] ", regs[15]);
        fgets(buf, 255, stdin);
        parse_and_handle_input(buf, inferior);
    }
}

void symbol_table_reading_failed(FILE *f)
{
    if (f)
        fclose(f);

    fprintf(stderr, "Symbol table reading failed. No symbolic information will"
        " be\n");
    fprintf(stderr, "available for this run.\n");
}

int compare_symtab_indices(const void *va, const void *vb)
{
    struct weasel_symbol **a, **b;
    a = (struct weasel_symbol **)va, b = (struct weasel_symbol **)vb;
    return ((*a)->name.index - (*b)->name.index);
}

int compare_symtab_addrs(const void *va, const void *vb)
{
    struct weasel_symbol **a, **b;
    a = (struct weasel_symbol **)va, b = (struct weasel_symbol **)vb;
    return (((int)((*a)->addr)) - ((int)((*b)->addr)));
}

void read_symbol_table(char *path)
{
    char *strtab;
    int i;
    struct load_command lc;
    struct mach_header mh;
    struct nlist sym;
    struct symtab_command sc;
    FILE *f = NULL;

    f = fopen(path, "r");
    if (!f) {
        fprintf(stderr, "Failed to open Mach-O binary '%s'.\n", path);
        symbol_table_reading_failed(f);
        return;
    }

    fread(&mh, sizeof(struct mach_header), 1, f);
    if (mh.magic != MH_MAGIC) {
        fprintf(stderr,
            "Doesn't seem to be a Mach-O file I know how to deal with.\n"); 
        symbol_table_reading_failed(f);
        return;
    }

    for (i = 0; i < mh.ncmds; i++) {
        fread(&lc, sizeof(struct load_command), 1, f);
        if (lc.cmd != LC_SYMTAB) {
            fseek(f, lc.cmdsize - sizeof(struct load_command), SEEK_CUR);
            continue;
        }

        break;
    }

    if (lc.cmd != LC_SYMTAB) {
        symbol_table_reading_failed(f);
        return;
    }

    fseek(f, -sizeof(struct load_command), SEEK_CUR);
    fread(&sc, sizeof(struct symtab_command), 1, f);

    symbol_table_size = 0; 
    symbol_table = (struct weasel_symbol **)malloc(sizeof(struct weasel_symbol
        *) * sc.nsyms);

    fseek(f, sc.symoff, SEEK_SET);
    for (i = 0; i < sc.nsyms; i++) {
        fread(&sym, sizeof(struct nlist), 1, f);
        if (((sym.n_type) & N_TYPE) != N_SECT)
            continue;

        symbol_table[symbol_table_size] = (struct weasel_symbol *)malloc(
            sizeof(struct weasel_symbol)); 
        symbol_table[symbol_table_size]->addr = sym.n_value;
        symbol_table[symbol_table_size]->name.index = sym.n_un.n_strx;

        symbol_table_size++;
    }

    qsort(symbol_table, symbol_table_size, sizeof(struct weasel_symbol *),
        compare_symtab_indices);
    strtab = (char *)malloc(sc.strsize);
    fseek(f, sc.stroff, SEEK_SET);
    fread(strtab, 1, sc.strsize, f); 

    for (i = 0; i < symbol_table_size; i++)
        symbol_table[i]->name.str = strdup(strtab +
            symbol_table[i]->name.index);

    free(strtab);
    fclose(f);

    qsort(symbol_table, symbol_table_size, sizeof(struct weasel_symbol *),
        compare_symtab_addrs);

    fprintf(stderr, "Read %d symbols.\n", symbol_table_size);
}

int main(int argc, char **argv)
{
    pid_t inferior;

    if (argc < 2) {
        fprintf(stderr, "usage: %s executable-path [args...]\n", argv[0]);
        return 0;
    }

    read_symbol_table(argv[1]);
    inferior = start_inferior(argc - 1, argv + 1);
    attach_to_process(inferior);
    main_loop(argv[1], inferior);
    
    return 0;
}

