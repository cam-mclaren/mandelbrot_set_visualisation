#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <gmp.h>
#include <mpfr.h>
#include "my_complex.h"


/* This file defines two structures used for complex number representations.
Complex is a structure built from the default double data types and is a
fairly staighforward data structure. Functions aimed at producing a complex
output should simply return the desired answer.

The second complex data structure, complex_big, does not use the default double data type.
Instead it uses the mpfr_t data type. I believe that mpfr_t variables cannot be
returned like non-array standard data types. This is because the mpfr_t data
types have more in common with arrays. Accordingly we adopt the mpfr convention
of designating the first argument of the function as the desired output location
for functions intended to have a complex_big output.




 */

 typedef struct complex{
   double real;
   double imaginary;
 } complex;

 typedef struct complex_big{
   mpfr_t real;
   mpfr_t imaginary;
 } complex_big;



void initialise_complex_big(complex_big number, int precision)
{
  mpfr_init2(number.real, precision);
  mpfr_init2(number.imaginary, precision);
}

void set_complex_big(complex_big number, mpfr_t real, mpfr_t imaginary)
{
  mpfr_set(number.real, real, MPFR_RNDD);
  mpfr_set(number.imaginary, imaginary, MPFR_RNDD);
}

void multiply_complex_big(complex_big answer, \
  complex_big argument1, \
  complex_big argument2, int precision){
    mpfr_t temp;
    mpfr_init2(temp, precision);
    mpfr_mul(answer.real, argument1.real, argument2.real, MPFR_RNDD);
    mpfr_mul(temp, argument1.imaginary, argument2.imaginary, MPFR_RNDD);
    mpfr_sub(answer.real, answer.real, temp, MPFR_RNDD);

    mpfr_mul(answer.imaginary, argument1.real, argument2.imaginary, MPFR_RNDD);
    mpfr_mul(temp, argument1.imaginary, argument2.real, MPFR_RNDD);
    mpfr_add(answer.imaginary, answer.imaginary, temp, MPFR_RNDD);
    mpfr_clear(temp);
  }

  void add_complex_big(complex_big answer,\
                       complex_big arg1,\
                       complex_big arg2)
  {

    mpfr_add(answer.real, arg1.real, arg2.real, MPFR_RNDD);
    mpfr_add(answer.imaginary, arg1.imaginary, arg2.imaginary, MPFR_RNDD);
  }


complex add_complex(complex arg1, complex arg2){
  complex output;
  output.real = arg1.real + arg2.real;
  output.imaginary = arg1.imaginary + arg2.imaginary;

  return output;
}



complex multiply_complex(complex arg1, complex arg2)
{
  complex answer;
  answer.real = arg1.real*arg2.real - arg1.imaginary*arg2.imaginary;
  answer.imaginary = arg1.real*arg2.imaginary + arg1.imaginary*arg2.real;

  return answer;
}


complex multiply_complex_big_and_small(struct complex_big big_arg, \
  complex small_arg)
{
  complex reduced, answer;
  reduced.real = mpfr_get_d(big_arg.real, MPFR_RNDD);
  reduced.imaginary = mpfr_get_d(big_arg.imaginary, MPFR_RNDD);

  answer= multiply_complex(reduced, small_arg);

  return answer;
}


complex add_complex_big_and_small(complex_big big_arg, \
  complex small_arg)
{
  complex reduced, answer;
  reduced.real = mpfr_get_d(big_arg.real, MPFR_RNDD);
  reduced.imaginary = mpfr_get_d(big_arg.imaginary, MPFR_RNDD);

  answer= add_complex(reduced, small_arg);

  return answer;
}

complex scale_complex(double scale_parameter, complex argument)
{
  complex output;

  output.real = scale_parameter*argument.real;
  output.imaginary = scale_parameter*argument.imaginary;

  return output;
}
