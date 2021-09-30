#include <math.h>
#include <stdlib.h>
// I may not need to include these... 

// I need to make the thing that actually calclates the coefficients 

/* 
nodes, y-values -> not sure how to stage the problem 
 tridiagonal solver 

option 1. Create a function that is a tridiagonal solver . Create a seperate piece of code that constructs a matix that the solver works with. 

option 2. Create the tridagonal solver and matrix constructor in one piece of code. 


nodes, y-values -> function -> coefficients    I quite like option 2 I think it would work better. 

Now the thing is... I actually know what my nodes are. The interpolating  range is  fromm 0 to 1. The first is of course zero. The second is 1/(number of y values-1), and so on. But I actually need a node on either side of the range. So I need to caluclate h... which is of course equal to the 2nd node... and have a negative h 

 */



// assume you have a vector of nodes and a vector of y_values




/*Section 1: Load the y values*/


FILE y_data_file = fopen(argc[1], 'r') 

// Count the number of values ( the y values 


// fill out the nodes 






double *nodes, *y_values, *rhs ;

int length;


double lower[length-1], middle[length], upper[length-1];

middle[length-1]=4; // assign the last element of middle to 4. The other assignments will be handeled in the following loop 


int i; 
for( i = 0; i<(length-1) ; i++){
	middle[i]=4;
	lower[i]=1;
	upper[i]=1;
}

// Now we have filled the diagonals... 
 

// initialise an output array for the solved values 

double *coefficients=malloc(sizeof(double)*(length+2));























//Alright so here is my polynomial. I think it should actually be quite fast because there isn't any loops or logical chains 

double interp_poly(double * coefficients, double * nodes, double h, double x)
{
	
	int k = floor( (x - nodes[0])/h)+2; // We add two instead of one because arrays begin their index at 0. In the text we used indexing starting at -1. 


	return coefficient[k-2]*pow((2-(x-nodes[k-2])/h),3) + \
		coefficient[k-1]*( 1 + 3*pow( (1-(x-nodes[k-1])/h),1) + 3*pow( (1-(x-nodes[k-1])/h),2) - 3*pow( 1 - (x-nodes[k-1])/h, 3) )+ \
		coefficient[k]*(1+3*pow( 1+ (x-nodes[k])/h, 1) + 3*pow( 1+ (x-nodes[k])/h, 2)-3*pow(1+(x-nodes[k])/h,3))+ \
		coefficient[k+1]*pow(2+(x-nodes[k+1])/h,3); 
}
