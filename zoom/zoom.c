#include <stdio.h>
#include <stdlib.h>
#include <math.h>





int main(int argc, char * * argv)
{
  // Coordinate to zoom in upon. (Centre of the final frame)
  double x;
  double y;

  // left and right boundaries of the first frame
  double left =-2
  double right =2;

  double first_frame_width = right - left;


  //Pixel data for image

  int x_pixel_count = 200;
  int y_pixel_count = 400;

  //Find aspect ratio
  double aspect_ratio = (double)x_pixel_count/y_pixel_count;

  // Find y boundaries for the intial frame
  double top = aspect_ratio*right;
  double bot = aspect_ratio*left;

  // Now that we have both the horizontal and vertical boundaries we can find the centre of the first frame.
  double first_frame_centre_x = (Right+Left)/2;
  double first_frame_centre_y = (Top+Bottom)/2;

  // Define an array to see how big we can make the final frame so that it touches only one boundary
  double * closeness = malloc(4*sizeof(double));
  closeness[0]=x-left;
  closeness[1]=right-x;
  closeness[2]=(y-bot)/aspect_ratio;
  closeness[3]=(top-y)/aspect_ratio;

  // find the minimum element of closeness
  int current_index=0;
  double current_min=closeness[0];
  int i;
  for( i=1; i<4;i++)
  {
    if(closeness[i]<current_min)
    {
      current_min=closeness[i];
      current_index=i;
    }
  }


// We can now calculate the width of the final frame
double final_frame_width = 2*current_min;

int n = floor(log(first_frame_width/final_frame_width)/log(1.025))+1;


// initialise arrays for the frame specs. We will unpack these backwards starting from n-1 and going to 0
double * c_x=malloc(n*sizeof(double)); // centre x coordinate
double * c_y= malloc(n*sizeof(double)); // centre y coordinate
double * multiplier = malloc(n*sizeof(double));  // Multiplier of the desired final frame width

// We can define the gradient parametes of the 2 line equations
double gradient_y = (first_frame_centre_y-y)/(first_frame_centre_x-x);
double gradient_mult = ((first_frame_width-final_frame_width)/(first_frame_centre_x-x);

// Populate frame parameter arrays
for(i = 0 ; i < n; i++)
{
  c_x[i] = x+ (first_frame_centre_x-x)/(n-1)*i;
  c_y[i] = gradient_y*(c_x[i]-x)+y;
  multiplier[i] = gradient_mult*(c_x[i]-x)+final_frame_width;
}


double * base_x_values=malloc(x_pixel_count*sizeof(double));
double * base_y_values=malloc(y_pixel_count*sizeof(double));

// Populate the base values


for( i = 0; i <x_pixel_count; i++)
{
  base_x_values[i]= -0.5 + (1)/(x_pixel_count-1)*i;
}
for( i = 0; i <y_pixel_count; i++)
{
  base_y_values[i]= -0.5*aspect_ratio + (1*aspect_ratio)/(y_pixel_count-1)*i;
}







}
