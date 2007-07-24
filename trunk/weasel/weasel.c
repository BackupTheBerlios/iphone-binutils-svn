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
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>

/* TODO: make a sys header for this guy */
kern_return_t task_threads(task_t target_task, thread_array_t *act_list,
    mach_msg_type_number_t *act_listCnt);

int inferior_in_ptrace = 1;
task_t inferior_task;
thread_array_t inferior_threads = NULL;
unsigned int inferior_thread_count = 0;

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

    err = vm_write(inferior_task, addr, &inst, 4);
    if (err == KERN_SUCCESS)
        fprintf(stderr, "Breakpoint set at $%08x.\n", addr);
    else
        fprintf(stderr, "Failed to set breakpoint at $%08x: %d.\n", addr,
            (int)err);
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
        case 'q':
            ptrace(PT_KILL, inferior, 0, 0);
            wait(&status);
            exit(0);
            break;
        default:
            fprintf(stderr, "Available weasel commands:\n");
            fprintf(stderr, "    b set a breakpoint\n");
            fprintf(stderr, "    c continue execution\n");
            fprintf(stderr, "    d disassemble starting at address or last\n");
            fprintf(stderr, "    q quit weasel and inferior\n");
    }
}

void main_loop(char *inferior_name, pid_t inferior)
{
    char buf[256];
    unsigned int regs[17];

    while (1) {
        get_inferior_status(regs);
        printf("[$%08x weasel] ", regs[15]);
        fflush(stdout);
        fgets(buf, 255, stdin);
        parse_and_handle_input(buf, inferior);
    }
}

int main(int argc, char **argv)
{
    pid_t inferior;

    if (argc < 2) {
        fprintf(stderr, "usage: %s executable-path [args...]\n", argv[0]);
        return 0;
    }

    inferior = start_inferior(argc - 1, argv + 1);
    attach_to_process(inferior);
    main_loop(argv[1], inferior);
    
    return 0;
}

