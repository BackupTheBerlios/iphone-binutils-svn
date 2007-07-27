/* ----------------------------------------------------------------------------
 *   weasel - a minimalist debugger for Mac OS X           v0.01 - 07/23/2007
 *   Copyright (c) 2007 Patrick Walton but freely redistributable under the
 *   terms specified in the COPYING file.
 * ------------------------------------------------------------------------- */

#include <ctype.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <mach/exception_types.h>
#include <mach/mach_init.h>
#include <mach/mach_port.h>
#include <mach/mach_traps.h>
#include <mach/task.h>
#include <mach/task_info.h>
#include <mach/thread_act.h>
#include <mach/thread_info.h>
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

struct weasel_breakpoint {
    int id;
    unsigned int addr;
    unsigned int orig_inst;

    struct weasel_breakpoint *next;
};

/* TODO: make a sys header for this guy */
kern_return_t task_threads(task_t target_task, thread_array_t *act_list,
    mach_msg_type_number_t *act_listCnt);

int inferior_in_ptrace = 1;
int next_breakpoint_id = 1;
unsigned int inferior_thread_count = 0;
unsigned int last_disassembly_point = 0, last_peek_point = 0;
unsigned int symbol_table_size = 0;
struct weasel_breakpoint *breakpoints = NULL;
struct weasel_symbol **symbol_table = NULL;
mach_port_t exception_port; 
exception_type_t exception_type;
exception_data_t exception_data;
task_t inferior_task;
thread_array_t inferior_threads = NULL;

void inferior_abort_handler(int pid)
{
    fprintf(stderr, "Inferior received signal SIGABRT. Executing BKPT.\n");
    asm("bkpt 0");
}

void child_handler(int pid)
{
    int status;

    wait(&status);
    if (WIFEXITED(status)) {
        fprintf(stderr, "Inferior exited with status %d.\n", (int)
            WEXITSTATUS(status));
        exit(0);
    } else if (WIFSIGNALED(status)) {
        fprintf(stderr, "Inferior exited with signal %d.\n", (int)WTERMSIG(
            status));
        exit(0);
    }
}

pid_t start_inferior(int argc, char **argv)
{
    char **child_args;
    int status;
    pid_t kid;

    fprintf(stderr, "Starting process...\n");

    if ((kid = fork())) {
        wait(&status);
        if (WIFSTOPPED(status)) {
            fprintf(stderr, "Process with PID %d started...\n", (int)kid);
            return kid;
        }

        exit(0);
    }

    child_args = (char **)malloc(sizeof(char *) * (argc + 1));
    memcpy(child_args, argv, sizeof(char *) * argc);
    child_args[argc] = NULL;

    ptrace(PT_TRACE_ME, 0, 0, 0);
    signal(SIGTRAP, SIG_IGN);
    signal(SIGABRT, inferior_abort_handler);

    execvp(argv[0], child_args);

    fprintf(stderr, "Failed to start inferior.\n");
    exit(1);

    return 0;   /* should never get here */
}

void attach_to_process(pid_t inferior)
{
    kern_return_t err;

    signal(SIGCHLD, child_handler);

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

    if (mach_port_allocate(mach_task_self(), MACH_PORT_RIGHT_RECEIVE,
        &exception_port) != KERN_SUCCESS) {
        fprintf(stderr, "Failed to create exception port.\n");
        exit(1);
    }
    if (mach_port_insert_right(mach_task_self(), exception_port,
        exception_port, MACH_MSG_TYPE_MAKE_SEND) != KERN_SUCCESS) {
        fprintf(stderr, "Failed to acquire insertion rights on the port.\n");
        exit(1);
    }
    if (task_set_exception_ports(inferior_task, EXC_MASK_ALL, exception_port,
        EXCEPTION_DEFAULT, THREAD_STATE_NONE) != KERN_SUCCESS) {
        fprintf(stderr, "Failed to set the inferior's exception ports.\n");
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
    unsigned int inst, size;
    struct weasel_breakpoint *bkpt;

    err = vm_protect(inferior_task, addr, 4, 0, VM_PROT_READ | VM_PROT_WRITE |
        VM_PROT_EXECUTE); 
    if (err != KERN_SUCCESS) {
        fprintf(stderr, "Failed to unprotect memory: %d.\n", err);
        return;
    }

    bkpt = (struct weasel_breakpoint *)malloc(sizeof(struct
        weasel_breakpoint));
    bkpt->id = next_breakpoint_id;
    bkpt->addr = addr;

    size = 4;
    err = vm_read_overwrite(inferior_task, addr, 4, (pointer_t)&(
        bkpt->orig_inst), &size);
    if (err != KERN_SUCCESS) {
        fprintf(stderr, "Failed to read instruction!\n");
        free(bkpt);
        return;
    }

    bkpt->next = breakpoints;
    breakpoints = bkpt;

    /* We know this is in the right endianness because it's our own! */
    inst = ((0xe12 << 20) | (0x7 << 4));

    err = vm_write(inferior_task, addr, (pointer_t)&inst,
        (mach_msg_type_number_t)4);
    if (err == KERN_SUCCESS)
        fprintf(stderr, "Breakpoint %d set at $%08x.\n", next_breakpoint_id,
            addr);
    else
        fprintf(stderr, "Failed to set breakpoint at $%08x: %d.\n", addr,
            (int)err);

    next_breakpoint_id++;
}

void do_disassembly(unsigned int addr)
{
    char *assembler;
    unsigned int i, inst, size;
    kern_return_t err;

    err = vm_protect(inferior_task, addr, 4 * 22, 0, VM_PROT_READ |
        VM_PROT_WRITE | VM_PROT_EXECUTE); 
    if (err != KERN_SUCCESS) {
        fprintf(stderr, "Failed to unprotect memory: %d.\n", err);
        return;
    }

    for (i = addr; i < addr + 4 * 22; i += 4) {
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

void do_peek(unsigned int addr)
{
    unsigned char data[16];
    unsigned int i, j, size;
    kern_return_t err;

    err = vm_protect(inferior_task, addr, 16 * 21, 0, VM_PROT_READ |
        VM_PROT_WRITE | VM_PROT_EXECUTE); 
    if (err != KERN_SUCCESS) {
        fprintf(stderr, "Failed to unprotect memory: %d.\n", err);
        return;
    }
   
    fprintf(stderr, " address ");
    for (i = addr; i < addr + 16; i++)
        fprintf(stderr, "  %01x", i % 16);
    fprintf(stderr, "  ");
    for (i = addr; i < addr + 16; i++)
        fprintf(stderr, "%01x", i % 16);
    fprintf(stderr, "\n");
    
    for (i = addr; i < addr + 16 * 21; i += 16) {
        size = 16;
        err = vm_read_overwrite(inferior_task, i, 16, (pointer_t)data, &size);
        if (err != KERN_SUCCESS) {
            fprintf(stderr, "%08x  -- inaccessible --\n", i);
            continue;
        }

        fprintf(stderr, "%08x  ", i);
        for (j = 0; j < 16; j++)
            fprintf(stderr, "%02x ", (unsigned int)(data[j]));
        fprintf(stderr, " ");
        for (j = 0; j < 16; j++)
            fprintf(stderr, "%c", isprint(data[j]) ? data[j] : '.');
        fprintf(stderr, "\n");
    }

    last_peek_point = i;
}

void nm()
{
    int i;

    for (i = 0; i < symbol_table_size; i++) {
        fprintf(stderr, "%08x %s\n", symbol_table[i]->addr,
            symbol_table[i]->name.str);
    }
}

void delete_breakpoint(int id)
{
    kern_return_t err;
    struct weasel_breakpoint *bkpt, *prev;

    bkpt = breakpoints; prev = NULL;
    while (bkpt) {
        if (bkpt->id == id) {
            if (prev)
                prev->next = bkpt->next;
            else
                breakpoints = bkpt->next;
            bkpt->next = NULL;
            break;
        }
    }

    if (!bkpt) {
        fprintf(stderr, "No such breakpoint %d.\n", id);
        return;
    }

    err = vm_protect(inferior_task, bkpt->addr, 4, 0, VM_PROT_READ |
        VM_PROT_WRITE | VM_PROT_EXECUTE); 
    if (err != KERN_SUCCESS) {
        fprintf(stderr, "Failed to unprotect memory: %d.\n", err);
        return;
    }

    vm_write(inferior_task, bkpt->addr, (pointer_t)&(bkpt->orig_inst),
        (mach_msg_type_number_t)4);

    fprintf(stderr, "Breakpoint %d deleted.\n", bkpt->id);

    free(bkpt);
}


kern_return_t catch_exception_raise(mach_port_t exception_port, mach_port_t
    thread, mach_port_t task, exception_type_t exception, exception_data_t
    code, mach_msg_type_number_t code_count)
{

    exception_type = exception;
    exception_data = code;

    if (thread_suspend(inferior_threads[0]) != KERN_SUCCESS)
        fprintf(stderr, "Failed to suspend!\n");
    thread_abort(inferior_threads[0]);

    return KERN_SUCCESS;
}

void wait_for_events()
{
    char data[1024];
    unsigned int gp_count, regs[17];
    kern_return_t err;
    mach_msg_header_t msg, out_msg;
    struct weasel_breakpoint *bkpt;

    fprintf(stderr, "Listening for exceptions.\n");

    err = mach_msg(&msg, MACH_RCV_MSG, 0, sizeof(data), exception_port, 0,
        MACH_PORT_NULL);
    if (err != KERN_SUCCESS) {
        fprintf(stderr, "Event listening failed.\n");
        return;
    }

    fprintf(stderr, "Exceptional event received.\n");

    exc_server(&msg, &out_msg);

    /* if (mach_msg(&out_msg, MACH_SEND_MSG, sizeof(out_msg), 0, MACH_PORT_NULL,
        0, MACH_PORT_NULL) != KERN_SUCCESS) {
        fprintf(stderr, "Failed to send exception reply!\n");
        exit(1);
    } */

    fprintf(stderr, "Inferior received exception %x, %x.\n", (unsigned int)
        exception_type, (unsigned int)exception_data);

    gp_count = 17;
    thread_get_state(inferior_threads[0], 1, (thread_state_t)regs, &gp_count);

    bkpt = breakpoints;
    while (bkpt) {
        if (bkpt->addr == regs[15]) {
            fprintf(stderr, "Breakpoint %d hit at $%08x.\n", bkpt->id,
                bkpt->addr);
            delete_breakpoint(bkpt->id);
            break;
        }

        bkpt = bkpt->next;
    }
}

void print_registers()
{
    unsigned int gp_count, regs[17];

    gp_count = 17;
    thread_get_state(inferior_threads[0], 1, (thread_state_t)regs, &gp_count);
    show_registers(regs);
}

void parse_and_handle_input(char *buf, pid_t inferior)
{
    int n;
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
            if (inferior_in_ptrace) {
                ptrace(PT_DETACH, inferior, 0, 0);
                inferior_in_ptrace = 0;
            } else
                thread_resume(inferior_threads[0]);
            fprintf(stderr, "Continuing.\n");
            wait_for_events();
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
        case 'p':
            if (sscanf(buf, "p %x", &addr) < 1)
                do_peek(last_peek_point);
            else
                do_peek(addr);
            break;
        case 'r':
            print_registers();
            break; 
        case 't':
            /* print_call_frame_traceback();    TODO */
            fprintf(stderr, "Sorry, call frame tracebacks unimplemented...\n");
            break;
        case 'q':
            task_terminate(inferior_task); 
            exit(0);
            break;
        case 'x':
            if (sscanf(buf, "x %d", &n) < 1)
                fprintf(stderr, "usage: x breakpoint-number\n");
            else
                delete_breakpoint(n);
            break;
        default:
            fprintf(stderr, "Available weasel commands:\n");
            fprintf(stderr,
                "    b set a breakpoint at the given address\n");
            fprintf(stderr,
                "    c continue execution\n");
            fprintf(stderr,
                "    d disassemble starting at the given address\n");
            fprintf(stderr,
                "         (if no address given, continues from last point)\n");
            fprintf(stderr,
                "    n print the symbol table of the main image\n");
            fprintf(stderr,
                "    p peek at memory starting at the given address\n");
            fprintf(stderr,
                "    q quit weasel and inferior\n");
            fprintf(stderr,
                "    r print the current values of the CPU registers\n");
            fprintf(stderr,
                "    x delete the breakpoint with the given number\n");
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

