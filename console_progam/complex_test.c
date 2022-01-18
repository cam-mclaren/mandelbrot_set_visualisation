#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>
#include <mpfr.h>
#include "my_complex.h"






int main(void)
{

  // TEST COMPLEX

  //test complex add

  complex z;
  z.real=1;
  z.imaginary=1;

  complex c;
  c.real =1;
  c.imaginary =1;

  c=add_complex(c, z);

  printf("c=%f+%fi\n", c.real, c.imaginary);

  return 0;
}
