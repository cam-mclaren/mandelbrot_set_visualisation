#include <stdio.h>
#include <gmp.h>
#include <mpfr.h>
#include <math.h>


void print_float(mpfr_t var){

  mpfr_out_str(stdout,10,0,var,MPFR_RNDD);
  putchar('\n');

}



void arctan(mpfr_t value, mpfr_t argument, int n, int accuracy){

  mpfr_t accumulator, numerator, denominator, neg_one ,sign; //initialise the mpfr variables
  int i;
  mpfr_init2(accumulator, accuracy);
  mpfr_set_d(accumulator, 0.0, MPFR_RNDD);
  mpfr_init2(numerator, accuracy);
  mpfr_init2(neg_one, accuracy);
  mpfr_init2(sign, accuracy);
  mpfr_set_d(neg_one,-1.0, MPFR_RNDD);
  mpfr_init2(denominator, accuracy);
  mpfr_set_d(denominator, -1.0, MPFR_RNDD);



  for(i=1; i<n; i++)
  {
    //arcsine can be approximated by an alternating infinite series of the reciporicals of the odd numbers.
    mpfr_set(numerator, argument, MPFR_RNDU);
    //print_float(numerator);
    mpfr_pow_si(numerator, numerator, (2*i-1), MPFR_RNDD);
    //print_float(numerator);
    mpfr_add_si(denominator,denominator, 2, MPFR_RNDD);
    //print_float(numerator);
    mpfr_pow_si(sign, neg_one, (i-1), MPFR_RNDD);
    mpfr_mul(numerator,numerator,sign, MPFR_RNDD);
    mpfr_div(numerator, numerator, denominator, MPFR_RNDD);
    mpfr_add(accumulator, accumulator, numerator, MPFR_RNDD);
  }

  mpfr_set(value,accumulator, MPFR_RNDD);
  mpfr_clear(accumulator);
  mpfr_clear(denominator);
  mpfr_clear(numerator);
}






int main(void){

  unsigned int i;
  int accuracy = 1000;
  mpfr_t accumulator, numerator, denominator, value1, value2, value3, value4;
  double arg=.5;

  mpfr_set_default_prec(accuracy);

  mpfr_init2(numerator, accuracy);
  mpfr_set_d(numerator, 4.0,MPFR_RNDD);
  mpfr_init2(accumulator,accuracy);
  mpfr_set_d(accumulator,0.0,MPFR_RNDD);
  mpfr_init2(denominator,accuracy);
  mpfr_set_d(denominator,1.0,MPFR_RNDD);
  mpfr_init2(value1, accuracy);
  mpfr_init2(value2, accuracy);
  mpfr_init2(value3, accuracy);
  mpfr_init2(value4, accuracy);



  // Set up function arguments
  mpfr_t denom1, denom2, num1, num2;

  mpfr_init_set_ui(denom1, 5,  MPFR_RNDD);
  mpfr_init_set_ui(denom2, 239,  MPFR_RNDD);
  mpfr_init_set_ui(num1, 1,  MPFR_RNDD);
  mpfr_init_set_ui(num2, 1,  MPFR_RNDD);

  mpfr_div(num1, num1, denom1, MPFR_RNDD);
  mpfr_div(num2, num2, denom2, MPFR_RNDD);

// Print out a value of arctan
int reps=10000;

  arctan(value1, num1, reps, accuracy);
  arctan(value2, num2, reps,accuracy);



  mpfr_mul_d(value1, value1, 4.0, MPFR_RNDD);
  mpfr_sub(value1, value1, value2, MPFR_RNDD);
  mpfr_mul_d(value1,value1,4.0,MPFR_RNDD);
  //fprintf(stdout, "The arctan approx of %lf is ",arg);
  mpfr_printf("%.5Rf\n", value1);
  //mpfr_out_str(stdout,10,0,value1,MPFR_RNDD);
  putchar('\n');


  FILE * fptr;

  fptr=fopen("number.txt","w");


  mpfr_out_str(fptr, 10, 0, value1, MPFR_RNDD);
  fclose(fptr);


  mpfr_t input;
  mpfr_init2(input, 1000);

  fptr=fopen("number1.txt","r");

  mpfr_inp_str(input,fptr,10,MPFR_RNDD);

  fclose(fptr);

  printf("The read input is");
  print_float(input);



// End of Arctan code
printf("3.1415926535897932384626433832795028841971693993751058209\n");

  for(i=0; i <2000; i++)
  {
    mpfr_div(numerator, numerator, denominator, MPFR_RNDD);
    mpfr_add(accumulator, accumulator, numerator, MPFR_RNDD);
    mpfr_set_d(numerator, 4.0, MPFR_RNDD);
    mpfr_mul_d(numerator, numerator, pow(-1.0, i+1), MPFR_RNDD);
    mpfr_add_d(denominator,denominator,2.0,MPFR_RNDD);
  }

  //fprintf(stdout, "The sum is ");
  //mpfr_out_str(stdout,10,0,accumulator,MPFR_RNDD);
  putchar('\n');
  mpfr_clear(accumulator);
  mpfr_clear(denominator);
  mpfr_clear(numerator);
  mpfr_clear(value1);
  mpfr_clear(value2);
  mpfr_clear(value3);
  mpfr_free_cache();

  return 0;

}
