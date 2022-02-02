#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>
#include <mpfr.h>
#include "my_complex.h"
#include <time.h>




void print_complex(complex_big number)
{
  mpfr_fprintf(stdout, "%Rf + %Rfi\n", number.real, number.imaginary);
}

void print_complex_s(complex number)
{
  fprintf(stdout, "%lf+%lfi\n", number.real, number.imaginary);
}




int main(void)
{

  // TEST COMPLEX

  //test complex add

  complex z;
  z.real=6;
  z.imaginary=1;

  complex c;
  c.real =1;
  c.imaginary =1;

  c=add_complex(c, z);

  //printf("complex_add test\nAnswer: 7+2i\nResult:c=%f+%fi\n", c.real, c.imaginary);

  // text complex multiply

  z.real=3;
  z.imaginary=2;
  c.real=6;
  c.imaginary=-7;

  c=multiply_complex(c,z);
  //printf("multiply_complex test\nAnswer: 32-9i\nResult:c=%f+%fi\n", c.real, c.imaginary);


  //test complex scale

  c=scale_complex(2,c);
  //printf("scale_complex test\nAnswer: 64-18i\nResult:c=%f+%fi\n", c.real, c.imaginary);


  mpfr_t quantity1, quantity2;
  mpfr_init2(quantity1, MY_PREC);
  mpfr_init2(quantity2, MY_PREC);
  mpfr_set_d(quantity1, 1, MPFR_RNDD);
  mpfr_set_d(quantity2, 3, MPFR_RNDD);

  mpfr_t quantity3, quantity4;
  mpfr_init2(quantity3, MY_PREC);
  mpfr_init2(quantity4, MY_PREC);
  mpfr_set_d(quantity3, 1, MPFR_RNDD);
  mpfr_set_d(quantity4, 2, MPFR_RNDD);

  //mpfr_fprintf(stdout, "%Rf\n%Rf\n", quantity1, quantity2);

  complex_big y;
  complex_big x;
  complex_big ans;

  mpfr_t empty_quantity1, empty_quantity2;
  mpfr_init2(empty_quantity1, MY_PREC);
  mpfr_init2(empty_quantity1, MY_PREC);
  mpfr_set_d(empty_quantity1, 0, MPFR_RNDD);
  mpfr_set_d(empty_quantity2, 0, MPFR_RNDD);


  set_complex_big(&ans, empty_quantity1, empty_quantity2);
  printf("why\n");
  set_complex_big(&y, quantity1, quantity2);
  set_complex_big(&x, quantity3, quantity4);

  //mpfr_fprintf(stdout, "%Rf\n%Rf\n", y.real, y.imaginary);
  print_complex(x);
  print_complex(y);

  printf("complex_big_add test should yield 2+5i\n");
  add_complex_big(&ans, x,y);
  print_complex(ans);

  printf("multiply_complex_big test should yield -5+5i\n");
  multiply_complex_big(&ans, x,y);
  print_complex(ans);

  printf("square_complex_big test should yield -3+4i\n");
  square_complex_big(&ans, x);
  print_complex(ans);

  complex small_y, small_ans;
  small_y.real = 1;
  small_y.imaginary = 3;


  printf("add_complex_big_and_small should yield 2+5i\n");
  small_ans=add_complex_big_and_small(&x,small_y);
  print_complex_s(small_ans);

  printf("multiply_complex_big_and_small should yield -5+5i\n");
  small_ans=multiply_complex_big_and_small(&x,small_y);
  print_complex_s(small_ans);



  small_y.imaginary=0;
  int i, n=1000000000;




  mpfr_clear(y.real);
  mpfr_clear(y.imaginary);
  mpfr_free_cache();


  return 0;
}
