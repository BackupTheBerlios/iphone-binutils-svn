/* ----------------------------------------------------------------------------
 *   weasel - a minimalist debugger for Mac OS X           v0.01 - 07/23/2007
 *   Copyright (c) 2007 Patrick Walton but freely redistributable under the
 *   terms specified in the COPYING file.
 * ------------------------------------------------------------------------- */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <mach/mach_init.h>
#include <mach/mach_traps.h>
#include <mach/thread_act.h>
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>

task_t inferior_task;

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

    child_args = (char **)malloc(sizeof(char *) * argc);
    memcpy(child_args, argv + 1, sizeof(char *) * (argc - 1));
    child_args[argc - 1] = NULL;

    ptrace(PT_TRACE_ME, 0, 0, 0);
    execvp(argv[0], child_args);

    fprintf(stderr, "Failed to start inferior.\n");
    exit(1);

    return 0;   /* should never get here */
}

void attach_to_process(pid_t inferior)
{
    kern_return_t err;
    thread_array_t threads = NULL;
    unsigned int i, thread_count = 0;

    if ((err = task_for_pid(mach_task_self(), (int)inferior, &inferior_task) !=
        KERN_SUCCESS)) {
        fprintf(stderr, "Failed to get task for pid %d: %d.\n", (int)inferior,
            (int)err);
        exit(1);
    }

    if (task_threads(inferior_task, &threads, &thread_count) != KERN_SUCCESS) {
        fprintf(stderr, "Failed to get list of task's threads.\n");
        exit(1);
    } else
        fprintf(stderr, "Target has %d thread(s).\n", (int)thread_count);

    for (i = 0; i < thread_count; i++) {
        unsigned int gp_regs[32];
        unsigned int gp_count = 32;
     
        if ((err = thread_get_state(threads[i], 1, (thread_state_t) &gp_regs,
            &gp_count)) != KERN_SUCCESS) {
            fprintf(stderr, "Failed to get thread %d state (%d).\n", (int)i,
                (int)err);
            exit(1);
        }

        fprintf(stderr, "Thread %d: got regs.\n", (int)i); 
    } 
}

void main_loop(char *inferior_name, pid_t inferior)
{
    // printf("(%s@$%08x weasel) ", inferior_name);
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

