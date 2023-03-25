#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>
#include <mpfr.h>





int main(int argc, char * * argv){

  // first argument is the x pixel, second the y pixel

  // I need the number of pixels in the image to make this work. For now I will just read out of single_image.c but
  //   I will need a way of stitching this together

  /* Read in image parameter file */
  if (argc!=3)
  {
    fprintf(stderr, "Error. Incompatible number of arguments to find new centre.\n");
    return EXIT_FAILURE;
  }
  int x_pixels = 2000;//1080;
  int y_pixels = 2000;//1350;

  // Read from file the coordinates

  mpfr_t x_coordinate, y_coordinate, width;
  int precision=665;
  mpfr_init2(x_coordinate, precision);
  mpfr_init2(y_coordinate, precision);
  mpfr_init2(width, precision);
  int read_status;

  FILE * fptr=fopen("image_parameters.txt", "r");
  if(fptr==NULL)
  {
    fprintf(stderr, "Failed to open file\n");
    return EXIT_FAILURE;
  }
  read_status=mpfr_inp_str(x_coordinate, fptr, 10, MPFR_RNDD);
  if(read_status==0)
  {
    fprintf(stderr,"Error. mpfr_inp_str() failed to read x_coordinate\n");
    fclose(fptr);
    return EXIT_FAILURE;
  }
  read_status=mpfr_inp_str(y_coordinate, fptr, 10, MPFR_RNDD);
  if(read_status==0)
  {
    fprintf(stderr,"Error. mpfr_inp_str() failed to read y_coordinate\n");
    fclose(fptr);
    return EXIT_FAILURE;
  }
  read_status=mpfr_inp_str(width, fptr, 10, MPFR_RNDD);
  if(read_status==0)
  {
    fprintf(stderr,"Error. mpfr_inp_str() failed to read width\n");
    fclose(fptr);
    return EXIT_FAILURE;
  }
  fclose(fptr);


  /* Image parameter file has been read */

  // calculate aspect ratio
  mpfr_t aspect_ratio;
  mpfr_init2(aspect_ratio, precision);
  mpfr_set_si(aspect_ratio, y_pixels, MPFR_RNDD);
  mpfr_div_si(aspect_ratio, aspect_ratio, x_pixels, MPFR_RNDD);



  /* Find height, left boundary and bottom boundary */

  mpfr_t height, half_height, half_width, bottom_boundary, left_boundary;
  mpfr_init2(height, precision);
  mpfr_init2(half_height, precision);
  mpfr_init2(half_width, precision);
  mpfr_init2(bottom_boundary, precision);
  mpfr_init2(left_boundary, precision);

  // Low hanging fruit first, calculate half_width
  mpfr_div_ui(half_width, width, 2, MPFR_RNDD);

  // Find Left boundary
  mpfr_sub(left_boundary, x_coordinate, half_width, MPFR_RNDD);

  // find height
  mpfr_mul(height, width, aspect_ratio, MPFR_RNDD);

  // find half height
  mpfr_div_ui(half_height, height, 2, MPFR_RNDD);

  // Find the bottom boundary
  mpfr_sub(bottom_boundary, y_coordinate, half_height, MPFR_RNDD);


  /* Calculate offset */

  int x_pixel_target=atoi(argv[1]);
  int y_pixel_target=atoi(argv[2]);

  mpfr_t x_offset, y_offset;
  mpfr_init2(x_offset, precision);
  mpfr_init2(y_offset, precision);

  mpfr_mul_ui(x_offset, width, x_pixel_target, MPFR_RNDD);
  mpfr_div_ui(x_offset, x_offset, x_pixels, MPFR_RNDD);

  mpfr_mul_ui(y_offset, height, y_pixel_target, MPFR_RNDD);
  mpfr_div_ui(y_offset, y_offset, y_pixels, MPFR_RNDD);



  /* Calculate the new centre coordinates */

  mpfr_t new_centre_x, new_centre_y;
  mpfr_init2(new_centre_x, precision);
  mpfr_init2(new_centre_y, precision);

  mpfr_add(new_centre_x, left_boundary, x_offset, MPFR_RNDD);
  mpfr_add(new_centre_y, bottom_boundary, y_offset, MPFR_RNDD);


  //print this out to see if it works.
  mpfr_fprintf(stdout, "%.30Rf\n", new_centre_x );
  mpfr_fprintf(stdout, "%.30Rf\n", new_centre_y );


  return 0;

}
