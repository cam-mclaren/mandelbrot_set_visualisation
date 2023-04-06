#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <gmp.h>
#include <mpfr.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include <threads.h>
#define MY_PRECISION 665



typedef struct image_params 
{
	int y_pixels;
	int x_pixels;
	int * row_count;
	unsigned char * image_data;
	mpfr_t * x_array;
	mpfr_t * y_array;
	mtx_t * mutex_ptr;
} image_params;

// You will need to import the colour arrays you need.

// A function to print arrays of doubles
void print_double_array(double *array, int size);

// This function supplied with the right coefficients and node arguments should give the value of the cubic B spline at x
double interp_poly(double * coefficients, double * nodes, double x);

// Function to count the number of lines in a file of doubles (Assumes one double per line)
int count_doubles_in_file(const char * path);

void fill_double_array_from_file(const char * path, int size, double * target_array);

int worker_function( void * thread_arg /* void pointer to int */)
{/*
	* So its outputs go in image_data
	* It can be fed a void pointer which it can cast to an int pointer. It reads this int pointer so it knows 
	* what row it is working on. It increments this pointer once it is done. 
	*
	* Now it also has to take in the y value somehow. It then has to iterate over the x values. 
	*/

	// Declare local copies of necessary variables.
	int current_row;
	mpfr_t current_y, current_x;
	int r, g, b;
	double speed, inner_product;
	int max_iter=4000;
	int iter_count=0;
	mpfr_t real_component, imaginary_component, x_square, y_square, real_temp;
	mpfr_t c_x, c_y;
	mpfr_init2(real_component, MY_PRECISION);
	mpfr_init2(imaginary_component, MY_PRECISION);
	mpfr_init2(real_temp, MY_PRECISION);
	mpfr_init2(x_square, MY_PRECISION);
	mpfr_init2(y_square, MY_PRECISION);
	mpfr_init2(current_y,MY_PRECISION);
	mpfr_init2(current_x,MY_PRECISION);

	image_params *args = (image_params*)thread_arg;


	int count = 0;
	printf("args->ypixels = %d\n", args->y_pixels);
	while (count < args->y_pixels)
	{
		


		mtx_lock(args->mutex_ptr);
		current_row = *(args->row_count); // read current row
		printf("current row = %d\n", current_row);
		if (current_row == args->y_pixels -1)
		{	
			printf("Worker function is returning\n");
			return 0;
		} else
		{
			*(args->row_count) = current_row+1;
			printf("incrementing\n");
		}
		mtx_unlock(args->mutex_ptr);

		mpfr_set(current_y, args->y_array[current_row], MPFR_RNDD);

		mpfr_fprintf(stdout,"%5.50Rf\n",current_y);
		printf("Writing X values from worker function\n");
		int j;
		for(j = 0; j< args->x_pixels; j++)
				{	

					//mpfr_fprintf(stdout,"%5.50Rf\n",args->x_array[j]);
					// mpfr_set(current_x, args->x_array[j], MPFR_RNDD);
					// mpfr_set(real_component, current_x, MPFR_RNDD); //  Initialise x value
					// mpfr_set(imaginary_component, current_y, MPFR_RNDD); // Initialise y value
					// run time escape algorithm
					// inner_product=0;
					// iter_count=0;
					// while( (iter_count < max_iter)&&(inner_product<4))
					// {
						// f(z) = z^2 + c
						// (x+yi)(x+yi) = x^2 - y^2 + (2xy)i
						//Real component calculation store in real_temp
						// mpfr_pow_si(x_square, real_component, 2, MPFR_RNDD);
						// mpfr_pow_si(y_square, imaginary_component, 2, MPFR_RNDD);
						// mpfr_sub(real_temp, x_square, y_square, MPFR_RNDD);
						//imaginary_component
						// mpfr_mul(imaginary_component, imaginary_component, real_component, MPFR_RNDD);
						// mpfr_mul_si(imaginary_component, imaginary_component, 2, MPFR_RNDD); // imaginary component calculate
						// add c
						// mpfr_add(real_component, real_temp, current_x, MPFR_RNDD);
						// mpfr_add(imaginary_component, imaginary_component, current_y, MPFR_RNDD);
						//find the inner product
						// mpfr_pow_si(x_square, real_component, 2, MPFR_RNDD);
						// mpfr_pow_si(y_square, imaginary_component, 2, MPFR_RNDD);
						// mpfr_add(real_temp, x_square, y_square, MPFR_RNDD);
						// inner_product=mpfr_get_d(real_temp, MPFR_RNDD);

						//if(iter_count%10==0)
						//{
							//printf("iter_count=%d, i=%d , j=%d \n",iter_count, i, j);
						//}
						// iter_count++;

					// }


				}
		count++;
	}
	fprintf(stderr, "Thread function loop maxed out");
	return -99;
}
// Instead maybe it reads targets from a file.



int main(void)
{

	// Read from file the coordinates

	mpfr_t x_coordinate, y_coordinate, width;
	int precision=1065;
	mpfr_init2(x_coordinate, MY_PRECISION);
	mpfr_init2(y_coordinate, MY_PRECISION);
	mpfr_init2(width, MY_PRECISION);
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

	//mpfr_fprintf(stdout,"%Rf\n",x_coordinate);
	//mpfr_fprintf(stdout,"%Rf\n",y_coordinate);
	//mpfr_fprintf(stdout,"%Rf\n",width);



	// Load the coefficents and nodes for the colouring spline//
	int coefficent_number = count_doubles_in_file("../interpolation/colour_parameters/blue_coefficients");
	double * red_coefficients=malloc(coefficent_number*sizeof(double));
	double * green_coefficients=malloc(coefficent_number*sizeof(double));
	double * blue_coefficients=malloc(coefficent_number*sizeof(double));
	double * nodes= malloc(coefficent_number*sizeof(double));

	fill_double_array_from_file("../interpolation/colour_parameters/red_coefficients",\
															coefficent_number,red_coefficients);
	fill_double_array_from_file("../interpolation/colour_parameters/green_coefficients",\
															coefficent_number,green_coefficients);
	fill_double_array_from_file("../interpolation/colour_parameters/blue_coefficients",\
															coefficent_number,blue_coefficients);
	fill_double_array_from_file("../interpolation/colour_parameters/nodes",\
															coefficent_number,nodes);


	//resolution
	int x_pixels = 200;
	int y_pixels = 200;

	// declare and initialise mutex
	mtx_t  mutex;
	if (mtx_init(&mutex, mtx_plain) != thrd_success)
	{
		fprintf(stderr, "Error initialising mutex on line %d\n", __LINE__);
		return -1;
	}
	fprintf(stdout, "Mutex successfully initialised at line %d\n", __LINE__);

	//define integer representing number of rows in image processed. Cast this as 
	//a void pointer. 
	int rows_processed = 0;
	void * thread_argument = (void*)&rows_processed;


	//aspect ratio
	mpfr_t aspect_ratio;
	mpfr_init2(aspect_ratio, MY_PRECISION);
	mpfr_set_si(aspect_ratio, y_pixels, MPFR_RNDD);
	mpfr_div_si(aspect_ratio, aspect_ratio, x_pixels, MPFR_RNDD);

	// delare and initialse variables for boundarys and nudges
	mpfr_t left, top, hoz_nudge, ver_nudge, half_height;
	mpfr_init2(left, MY_PRECISION);
	mpfr_init2(top, MY_PRECISION);
	mpfr_init2(hoz_nudge, MY_PRECISION);
	mpfr_init2(ver_nudge, MY_PRECISION);
	mpfr_init2(half_height, MY_PRECISION);

	mpfr_div_ui(hoz_nudge, width, 2, MPFR_RNDD); // use hoz nudge to store half width
	mpfr_sub(left, x_coordinate, hoz_nudge, MPFR_RNDD);  // left calculated
	mpfr_div_si(hoz_nudge, width, (x_pixels-1), MPFR_RNDD); //Now calculate the horizontal nudge

	mpfr_mul(half_height, aspect_ratio, width, MPFR_RNDD); //height calculated
	mpfr_div_si(ver_nudge, half_height, (y_pixels-1), MPFR_RNDD); // vertical nudge calculated
	mpfr_div_si(half_height, half_height, 2, MPFR_RNDD);
	mpfr_add(top, y_coordinate, half_height, MPFR_RNDD); // Top boundary calculated


	// Populate y values used. //Array is backwards atm :'-(
	//   __      __

	//    0		 0
	//    *	  ^  *
	//	  *      *
	//     ______
	/* 
	*/
	printf("top is:\n");
	mpfr_fprintf(stdout,"%5.50Rf\n",top);
	int index;
	mpfr_t y_val, y_increment;
	mpfr_t y_values[y_pixels];
	mpfr_init2(y_val, MY_PRECISION);
	mpfr_init2(y_increment, MY_PRECISION);
	for (index = y_pixels - 1; index >= 0 ; index -- )
	{
		mpfr_mul_si(y_increment, ver_nudge, (y_pixels -1 - index), MPFR_RNDD);
		mpfr_sub(y_val, top, y_increment, MPFR_RNDD);
		mpfr_init2(y_values[index], MY_PRECISION);
		mpfr_set(y_values[index], y_val, MPFR_RNDD);
		// mpfr_fprintf(stdout,"%5.50Rf\n",y_values[index]);
	}
	// Populate x values used. (Not yet utilised)
	mpfr_t x_val, x_increment;
	mpfr_t x_values[x_pixels];
	mpfr_init2(x_val, MY_PRECISION);
	mpfr_init2(x_increment, MY_PRECISION);
	for (index = 0 ; index < x_pixels; index ++ )
	{
		mpfr_mul_si(x_increment, hoz_nudge, index, MPFR_RNDD);
		mpfr_add(x_val, left, x_increment, MPFR_RNDD);
		mpfr_init2(x_values[index], MY_PRECISION);
		mpfr_set(x_values[index], x_val, MPFR_RNDD);
		// mpfr_fprintf(stdout,"%5.50Rf\n",x_values[index]);
	}



	mpfr_clear(half_height);
	mpfr_clear(aspect_ratio);
	mpfr_clear(x_coordinate);
	mpfr_clear(y_coordinate);
	mpfr_clear(width);

	printf("okay thus far\n");
	// define image_data array
	unsigned char * image_data = calloc(y_pixels*x_pixels*3, sizeof(unsigned char));

	mpfr_t current_x, current_y;
	mpfr_init2(current_x, MY_PRECISION);
	mpfr_init2(current_y, MY_PRECISION);
	mpfr_set(current_y, top, MPFR_RNDD); //initialise current y


	int r, g, b;
	double speed, inner_product;
	int max_iter=4000;
	int iter_count=0;
	mpfr_t real_component, imaginary_component, x_square, y_square, real_temp;
	mpfr_t c_x, c_y;
	mpfr_init2(real_component, MY_PRECISION);
	mpfr_init2(imaginary_component, MY_PRECISION);
	mpfr_init2(real_temp, MY_PRECISION);
	mpfr_init2(x_square, MY_PRECISION);
	mpfr_init2(y_square, MY_PRECISION);


	image_params thread_parameters;
	thread_parameters.x_pixels = x_pixels;
	thread_parameters.y_pixels = y_pixels;
	thread_parameters.row_count = &rows_processed;
	thread_parameters.mutex_ptr = &mutex;
	thread_parameters.image_data = image_data;
	thread_parameters.y_array = y_values;
	thread_parameters.x_array = x_values;

	worker_function( (void*) &thread_parameters);
	

	int i, j;
	for( i = (y_pixels - 1); i >= 0 ; i--)
	{	
		mpfr_set(current_y, y_values[i], MPFR_RNDD);
		printf("y=%d\n",i);
		for(j = 0; j< x_pixels; j++)
		{	
			mpfr_set(current_x, x_values[j], MPFR_RNDD);
			mpfr_set(real_component, current_x, MPFR_RNDD); //  Initialise x value
			mpfr_set(imaginary_component, current_y, MPFR_RNDD); // Initialise y value
			// run time escape algorithm
			inner_product=0;
			iter_count=0;
			while( (iter_count < max_iter)&&(inner_product<4))
			{
				// f(z) = z^2 + c
				// (x+yi)(x+yi) = x^2 - y^2 + (2xy)i
				//Real component calculation store in real_temp
				mpfr_pow_si(x_square, real_component, 2, MPFR_RNDD);
				mpfr_pow_si(y_square, imaginary_component, 2, MPFR_RNDD);
				mpfr_sub(real_temp, x_square, y_square, MPFR_RNDD);
				//imaginary_component
				mpfr_mul(imaginary_component, imaginary_component, real_component, MPFR_RNDD);
				mpfr_mul_si(imaginary_component, imaginary_component, 2, MPFR_RNDD); // imaginary component calculate
				// add c
				mpfr_add(real_component, real_temp, current_x, MPFR_RNDD);
				mpfr_add(imaginary_component, imaginary_component, current_y, MPFR_RNDD);
				//find the inner product
				mpfr_pow_si(x_square, real_component, 2, MPFR_RNDD);
				mpfr_pow_si(y_square, imaginary_component, 2, MPFR_RNDD);
				mpfr_add(real_temp, x_square, y_square, MPFR_RNDD);
				inner_product=mpfr_get_d(real_temp, MPFR_RNDD);

				//if(iter_count%10==0)
				//{
					//printf("iter_count=%d, i=%d , j=%d \n",iter_count, i, j);
				//}
				iter_count++;

			}

			speed = (double)iter_count/max_iter;


			if (speed==1)
			{
					// Does nothing. Pixel is initialised as black.
					// This is because calloc is zero initialised. (RGB 000 000 000 is black)
			}else
			{
				r = floor(interp_poly(red_coefficients, nodes, speed));
				g = floor(interp_poly(green_coefficients, nodes, speed));
				b = floor(interp_poly(blue_coefficients, nodes, speed));

				if( r > 255){ r=255;}
				if( g > 255){ g=255;}
				if( b > 255){ b=255;}
				if( r < 0){ r=0;}
				if( g < 0){ g=0;}
				if( b < 0){ b=0;}
			//printf("%d, %d, %d\n", r,g,b);


				*(image_data+(y_pixels - 1 - i)*x_pixels*3 + 3*j+0)=(unsigned char)r;
				*(image_data+(y_pixels - 1 - i)*x_pixels*3 + 3*j+1)=(unsigned char)g;
				*(image_data+(y_pixels - 1 - i)*x_pixels*3 + 3*j+2)=(unsigned char)b;
			}

		}
	}

	printf("fine till here\n" );
	stbi_write_jpg("image.jpg",x_pixels,y_pixels,3,image_data,100);
	printf("and here\n");
	free((void*)image_data);
	mpfr_clear(current_x);
	mpfr_clear(current_y);
	mpfr_clear(x_square);
	mpfr_clear(y_square);
	printf("and here\n");
	mpfr_clear(imaginary_component);
	mpfr_clear(real_component);
	mpfr_clear(real_temp);
	mpfr_clear(hoz_nudge);
	mpfr_clear(left);
	mpfr_clear(top);
	mpfr_clear(ver_nudge);
	printf("and here\n");
	mpfr_free_cache();


	return 0;
}


/*
// Begin filling out the image data using the escape time algorithm and interp polies
int j; //x index
//Escape time parameters
int max_iter =1000;
long double c_x, c_y, z_old_x, z_old_y, z_new_x, z_new_y;
int r,g, b;
double speed;
int iter_count;
for( i = 0 ; i<y_pixels; i++)
{
	c_y = y_array[i];
	for(j =0 ; j<x_pixels; j++)
	{
		c_x = x_array[j];
		z_old_x=0;
		z_old_y=0;
		//Escape Time Algorithm
		iter_count=0;
		while ((iter_count<max_iter)&(pow(z_old_x,2)+pow(z_old_y,2)<4))
		{
			iter_count++;
			z_new_x= pow(z_old_x,2)-pow(z_old_y,2) + c_x;
			z_new_y= 2*z_old_x*z_old_y + c_y;
			z_old_x=z_new_x;
			z_old_y=z_new_y;
		}

		speed = (double)iter_count/max_iter;
		if (speed==1){
		}else{
		r = floor(interp_poly(red_coefficients, nodes, speed));
		g = floor(interp_poly(green_coefficients, nodes, speed));
		b = floor(interp_poly(blue_coefficients, nodes, speed));

		if( r > 255){ r=255;}
		if( g > 255){ g=255;}
		if( b > 255){ b=255;}
		if( r < 0){ r=0;}
		if( g < 0){ g=0;}
		if( b < 0){ b=0;}
		//printf("%d, %d, %d\n", r,g,b);


		*(image_data+i*x_pixels*3 + 3*j+0)=(unsigned char)r;
		*(image_data+i*x_pixels*3 + 3*j+1)=(unsigned char)g;
		*(image_data+i*x_pixels*3 + 3*j+2)=(unsigned char)b;
	}
	}
}
stbi_write_jpg("image.jpg",x_pixels,y_pixels,3,image_data,100);
free(image_data);
return 0;
}*/










// This function supplied with the right coefficients and node arguments should give the value of the cubic B spline at x
double interp_poly(double * coefficients, double * nodes, double x)
{

	double h = nodes[1]-nodes[0];
	int k = floor( (x - nodes[1])/h)+2; // We add two instead of one because arrays begin their index at 0. In the text we used indexing starting at -1.


	return coefficients[k-2]*pow((2-(x-nodes[k-2])/h),3) + \
		coefficients[k-1]*( 1 + 3*pow( (1-(x-nodes[k-1])/h),1) + 3*pow( (1-(x-nodes[k-1])/h),2) - 3*pow( 1 - (x-nodes[k-1])/h, 3) )+ \
		coefficients[k]*(1+3*pow( 1+ (x-nodes[k])/h, 1) + 3*pow( 1+ (x-nodes[k])/h, 2)-3*pow(1+(x-nodes[k])/h,3))+ \
		coefficients[k+1]*pow(2+(x-nodes[k+1])/h,3);
}




// Function to count the number of lines in a file of doubles (Assumes one double per line)
int count_doubles_in_file(const char * path)
{
  FILE * file_pointer=fopen(path, "r");
  if (file_pointer==NULL)
  {
    fprintf(stderr,"Error in count_lines_in_file().\nFunction failed to open file.\n");
    return -1;
  }
  int line_count=0;
  double line_contents;
  while (fscanf(file_pointer,"%lf", &line_contents)==1)
  {
    line_count++;
  }
  fclose(file_pointer);
  return line_count;
}


void fill_double_array_from_file(const char * path, int size, double * target_array)
  {
    FILE * file_pointer = fopen(path,"r");
    int index;
    double num;
    for( index=0; index<size; index++)
    {
      fscanf(file_pointer, "%lf", &num);
      target_array[index]=num;
    }
    fclose(file_pointer);
  }


	void print_double_array(double *array, int size){
		int index;
		for( index=0; index<size; index++)
		{
			fprintf(stdout, "%f\n", array[index]);
		}
	}
