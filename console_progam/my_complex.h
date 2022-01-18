#ifndef MY_COMPLEX
#define MY_COMPLEX


#include <mpfr.h>
#include <gmp.h>
/*
typedef struct complex{
  double real;
  double imaginary;
} complex;

typedef struct complex_big{
  mpfr_t real;
  mpfr_t imaginary;
} complex_big;

complex add_complex(complex arg1, complex arg2){
  complex output;
  output.real = arg1.real + arg2.real;
  output.imaginary = arg1.imaginary + arg2.imaginary;

  return output;
}
*/
struct complex;

struct complex_big;

typedef struct complex complex;

typedef struct complex_big complex_big;

void initialise_complex_big(complex_big number, int precision);

void set_complex_big(complex_big number, mpfr_t real, mpfr_t imaginary);

void multiply_complex_big(complex_big answer, \
  complex_big argument1, \
  complex_big argument2, int precision);


void add_complex_big(complex_big answer,\
                       complex_big arg1,\
                       complex_big arg2);


complex add_complex(complex arg1, complex arg2);


complex multiply_complex(complex arg1, complex arg2);

complex multiply_complex_big_and_small(struct complex_big big_arg, \
  complex small_arg);

complex add_complex_big_and_small(complex_big big_arg, \
  complex small_arg);

complex scale_complex(double scale_parameter, complex argument);

#endif
