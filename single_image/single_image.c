#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"



// You will need to import the colour arrays you need.

// A function to print arrays of doubles
void print_double_array(double *array, int size){
	int index;
	for( index=0; index<size; index++)
	{
		fprintf(stdout, "%f\n", array[index]);
	}
}

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











int main(int argc, char * * argv)
{
/*
  int number = count_doubles_in_file("../interpolation/colour_parameters/blue_coefficients");
  double * my_array=malloc(number*sizeof(double));
  fill_double_array_from_file("../interpolation/colour_parameters/blue_coefficients", number, my_array);
	print_double_array(my_array, number);
  printf("%d\n",number);
  return 0;
*/

	/* Load the coefficents and nodes */
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

	/* determine image size */

	//resolution
	int x_pixels = 1080;
	int y_pixels = 1350;

	double aspect_ratio = (double)y_pixels/x_pixels;

	/* code x array and y array (actual number values) */
	double center_y = atof(argv[2]);
	double center_x = atof(argv[1]);
	double x_width=atof(argv[3]);
	double left = center_x-0.5*x_width;
	double right = center_x+0.5*x_width; ;


	double top = center_y + aspect_ratio*(center_x-left);
	double bot = center_y - aspect_ratio*(right-center_x);

	double * x_array = malloc(x_pixels*sizeof(double));
	double * y_array = malloc(y_pixels*sizeof(double));

	int i;
	double x_step = (right-left)/(x_pixels-1);
	for( i = 0 ; i <x_pixels; i++)
	{
		x_array[i]=left+i*x_step;
	}
	double y_step = (top-bot)/(y_pixels-1);
	for( i = 0 ; i <y_pixels; i++)
	{
		y_array[i]=bot+i*y_step;
	}

	/* define image_data array */

	unsigned char * image_data = calloc(y_pixels*x_pixels*3, sizeof(unsigned char));



/* Begin filling out the image data using the escape time algorithm and interp polies */
int j; //x index
//Escape time parameters
int max_iter =500;
double c_x, c_y, z_old_x, z_old_y, z_new_x, z_new_y;
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
/*
double colour;
int clo;
for( i = 0 ; i <x_pixels*y_pixels*3; i++)
{
	colour = (double)i/(x_pixels*y_pixels*3);
	clo = floor(200*colour);
	*(image_data+i) = (unsigned char)clo;
}
*/




stbi_write_jpg("image.jpg",x_pixels,y_pixels,3,image_data,100);
free(image_data);
return 0;


}
