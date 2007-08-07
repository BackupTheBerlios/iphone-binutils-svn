#include <limits.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#import <vmutils/vmutils.h>

NSSampler *sampler = nil;

void sigint_handler(int sig)
{
    if (!sampler)
        exit(0);

    fprintf(stderr, "Stopping sampling...\n");
    [sampler stopSampling];
    [sampler printStatistics];
    [sampler writeOutput: nil append: NO];
    fprintf(stderr, "Output written.\n");
    exit(0); 
}

int main(int argc, char **argv)
{
    int pid;
    unsigned int interval = 100;

    [[NSAutoreleasePool alloc] init];

    if (argc < 2) {
        fprintf(stderr, "usage: %s pid-to-sample\n", argv[0]);
        exit(1);
    }

    pid = strtol(argv[1], NULL, 0); 
    sampler = [[NSSampler alloc] initWithPid: pid];

    fprintf(stderr, "Sampling with interval %d...\n", (int)interval);

    signal(SIGINT, sigint_handler); 
    fprintf(stderr, "Use SIGINT (^C) to stop sampling.\n");

    [sampler startSamplingWithInterval: interval];

    while (1) {
        sleep(100);
    }

    return 0;
}

