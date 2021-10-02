
#include <stdio.h>
#include <math.h>
#include <stdlib.h>




void printarray(int *array, int size){
	int index;
	for( index=0; index<size; index++)
	{
		fprintf(stdout, "%d\n", array[index]);
	}
}

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


int main(int argc, char * * argv){

/*Section 1: Load the hex colour data */



// Check number of input arguments is correct

	if(argc!=2)
	{
		fprintf(stderr, "Error: Requires an input file of hex colour values\n");
		return EXIT_FAILURE;
	}


// Open file containing the colour data
	FILE * hex_data_file = fopen(argv[1], "r");
	if(hex_data_file==NULL)
	{
		fprintf(stderr, "fopen failed\n");
		return EXIT_FAILURE;
	}

	// Count the number of values ( the y values

	unsigned int value;
	int count=0;

	while(fscanf(hex_data_file, "%x", &value)==1)
	{
		count++;
	}


	// return to the top of the file

	fseek(hex_data_file, 0, SEEK_SET);



	// initialise arrays for the colour data

	int * red_array=malloc(count*sizeof(int));
	if( red_array==NULL)
	{
		fprintf(stderr, "malloc failed to allocate memory to the red array.\n");
		fclose(hex_data_file);
		return EXIT_FAILURE;
	}

	int * green_array=malloc(count*sizeof(int));
	if( green_array==NULL)
	{
		fprintf(stderr, "malloc failed to allocate memory to the green array.\n");
		fclose(hex_data_file);
		return EXIT_FAILURE;
	}

	int * blue_array=malloc(count*sizeof(int));
	if( blue_array==NULL)
	{
		fprintf(stderr, "malloc failed to allocate memory to the blue array.\n");
		fclose(hex_data_file);
		return EXIT_FAILURE;
	}


	// fill arrays


	int index;

	for( index=0; index<count; index++)
	{
		fscanf(hex_data_file, "%02x%02x%02x", (red_array+index),(green_array+index), (blue_array+index));
	}

	fclose(hex_data_file);

	fprintf(stdout, "red array\n");
	printarray(red_array, count);
	fprintf(stdout, "green array\n");
	printarray(green_array,count);
	fprintf(stdout, "blue array\n");
	printarray(blue_array,count);

	return 0;

}



/*


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


*/
