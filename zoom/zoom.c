#include <stdio.h>
#include <stdlib.h>
#include <math.h>





int main(int argc, char * * argv)
{
  // Coordinate to zoom in upon.
  double x;
  double y;

  // left and right boundaries of the first frame
  double left =-2
  double right =2;

  double first_frame_width = right - left;
  int n

  // initialise arrays for the frame specs. We will unpack these backwards starting from n-1 and going to 0
  double * c_x=malloc(n*sizeof(double)); // centre x coordinate
  double * c_y= malloc(n*sizeof(double)); // centre y coordinate
  double * multiplier = malloc(n*sizeof(double));  // Multiplier of the desired final frame width

  //Pixel data for image

  int x_pixel_count = 200;
  int y_pixel_count = 400;

  //Find aspect ratio
  double aspect_ratio = (double)x_pixel_count/y_pixel_count;

  // Find y boundaries for the intial frame
  double top = aspect_ratio*right;
  double bot = aspect_ratio*left;

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
}
