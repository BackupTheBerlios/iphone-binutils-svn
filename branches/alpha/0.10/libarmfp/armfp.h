#ifndef ARMFP_H
#define ARMFP_H

#define ARMFP_EXPORT __attribute__((visibility("default")))

double ARMFP_EXPORT __floatunsidf(unsigned int n);
float ARMFP_EXPORT __floatunsisf(unsigned int n);

/* External routines, in libc */
double __floatsidf(int n);
double __floatsisf(int n);

#endif

