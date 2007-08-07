#include "armfp.h"

double ARMFP_EXPORT __floatunsidf(unsigned int n)
{
    return __floatsidf((int)n);
}

float ARMFP_EXPORT __floatunsisf(unsigned int n)
{
    return __floatsisf((int)n);
}

