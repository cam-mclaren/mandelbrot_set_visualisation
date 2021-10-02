
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define PI 3.14159



void printarray(int *array, int size){
	int index;
	for( index=0; index<size; index++)
	{
		fprintf(stdout, "%d\n", array[index]);
	}
}

void print_double_array(double *array, int size){
	int index;
	for( index=0; index<size; index++)
	{
		fprintf(stdout, "%f\n", array[index]);
	}
}

void interpolating_coefficient_solver(double * y_data, int y_size, double* output_array)
{
	/*
	 c_0 = (first_value)/6 so that means c[1] cause c[0]=c_(-1),
	 c_(n) = (last_value)/6

	  */

		output_array[1] = y_data[0]/6;
		output_array[y_size]=y_data[y_size-1]/6;

		// allocate memory for tridagonal system

		double *upper =malloc((y_size-3)*sizeof(double));
		double *middle =malloc((y_size-2)*sizeof(double));
		double *lower =malloc((y_size-3)*sizeof(double));
		double *unknowns = malloc((y_size-2)*sizeof(double));
		double *rhs = malloc((y_size-2)*sizeof(double));

		// fill out coefficent matrix
		int system_order=y_size-2;
		int index;

		for( index = 0; index<system_order-1; index++)
		{
			upper[index]=1;
			middle[index]=4;
			lower[index]=1;
		}
		middle[system_order-1]=4;

		// fill out the rhs
		for( index=0; index <system_order; index++)
		{
			rhs[index]=y_data[index+1];
		}
		rhs[0]=rhs[0]-y_data[0]/6;

		rhs[system_order-1]=rhs[system_order-1]-y_data[y_size-1]/6;

		printf("rhs\n");
		print_double_array(rhs, system_order);
		printf("rhs\n");
		// eliminate the lower

		for( index =0; index<system_order-1; index++)
		{
			middle[index+1]=middle[index+1]-(lower[index]/middle[index])*upper[index];
			rhs[index+1]=rhs[index+1]-(lower[index]/middle[index])*rhs[index];
		}

		// solve for the unknowns

		unknowns[system_order-1]=rhs[system_order-1]/middle[system_order-1];

		for( index =system_order-2; index>-1; index--)
		{
			unknowns[index] = (rhs[index]-unknowns[index+1]*upper[index])/middle[index];
		}

		// so i have solved for c_0, c_1, ...., c_n

		for( index=2; index<y_size; index++)
		{
			output_array[index]=unknowns[index-2];
		}
		// I just need to solve for c_-1 and c_n+1
		output_array[0] = 2*output_array[1] - output_array[2];
		output_array[y_size+1]=2*output_array[y_size]-output_array[y_size-1];

		free(upper);
		free(middle);
		free(lower);
		free(rhs);
		free(unknowns);

}








// I may not need to include these...

// I need to make the thing that actually calclates the coefficients

/*
nodes, y-values -> not sure how to stage the problem
 tridiagonal solver

option 1. Create a function that is a tridiagonal solver . Create a seperate piece of code that constructs a matix that the solver works with.

option 2. Create the tridagonal solver and matrix constructor in one piece of code.


nodes, y-values -> function -> coefficients    I quite like option 2 I think it would work better.



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


	// fill  RGB arrays from the hex numbers in the input file.


	int index;

	for( index=0; index<count; index++)
	{
		fscanf(hex_data_file, "%02x%02x%02x", (red_array+index),(green_array+index), (blue_array+index));
	}

	fclose(hex_data_file);



	// Store colour information as doubles for the purpose of interpolationg. ie create some double arrays to store the colour info

	double * red_array_float =  malloc(count*sizeof(double));
	if( red_array_float==NULL){
		fprintf(stderr, "malloc failed to allocate memory to a double array to store colour info.\n");
		return EXIT_FAILURE;
	}

	double * green_array_float =  malloc(count*sizeof(double));
	if( green_array_float==NULL){
		fprintf(stderr, "malloc failed to allocate memory to a double array to store colour info.\n");
		return EXIT_FAILURE;
	}

	double * blue_array_float =  malloc(count*sizeof(double));
	if( blue_array_float==NULL){
		fprintf(stderr, "malloc failed to allocate memory to a double array to store colour info.\n");
		return EXIT_FAILURE;
	}


	for( index=0; index <count; index++)
	{
		red_array_float[index]=(double)red_array[index];
		green_array_float[index]=(double)green_array[index];
		blue_array_float[index]=(double)blue_array[index];
	}

	//free the dynamic memory storing the integer data
	free(red_array);
	free(green_array);
	free(blue_array);



	/* Section 2  Interpolation

	1. Creating the nodes in the domain (done)
	2. Creating the system -
		- tridiagonal system ( 3 vectors)

	Now the thing is... I actually know what my nodes are. The interpolating range is
	 fromm 0 to 1. The first is of course zero. The second is 1/(number of y values-1),
	  and so on. But I actually need a node on either side of the range.
		So I need to calculate h... which is of course equal to the 2nd node... and have a negative h

		nodes = [-h, 0, h, 2h, 3h, 1, 1+h]

	*/

	// build nodes
	double h = (double)1/(count-1);
	double * nodes = malloc((count+2)*sizeof(double));
	for( index =-1 ; index< (count+1); index ++)
	{
		nodes[index+1]= h*index;
	}

	// start filling in the coefficients
	double *coefficients_red = malloc((count+2)*sizeof(double));



	//////////////////////////// Deletable test of the intepolation function


	double some_values[5] = {2, (double)8/5,(double)8/6, (double)8/7, 1};
	double * some_coefficients = malloc(7*sizeof(double));

	interpolating_coefficient_solver(some_values, 5, some_coefficients);
	printf("coefficient outputs\n");
	print_double_array(some_coefficients,7);
	printf("some values\n");
	print_double_array(some_values, 5);



	///////////////////////////////////


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
