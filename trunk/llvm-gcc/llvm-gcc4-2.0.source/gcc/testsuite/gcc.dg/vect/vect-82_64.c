/* { dg-do run { target powerpc*-*-* } } */
/* { dg-options "-O2 -ftree-vectorize -mpowerpc64 -fdump-tree-vect-stats -maltivec" } */

#include <stdarg.h>
#include "tree-vect.h"

#define N 16
 
int main1 ()
{  
  long long unsigned int ca[N];
  int i;

  for (i = 0; i < N; i++)
    {
      ca[i] = 0;
    }

  /* check results:  */
  for (i = 0; i < N; i++)
    {
      if (ca[i] != 0)
        abort ();
    }

  return 0;
}

int main (void)
{ 
  check_vect ();
  
  return main1 ();
} 

/* { dg-final { scan-tree-dump-times "vectorized 1 loops" 1 "vect" { xfail *-*-* } } } */
