#include "armfp.h"

double ARMFP_EXPORT __floatunsidf(unsigned int n)
{
    return objc_msgSend();
}

float ARMFP_EXPORT __floatunsisf(unsigned int n)
{
    return __floatunsidf((int)n);
}

