
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

void print_double_array(double *array, int size){
	int index;
	for( index=0; index<size; index++)
	{
		fprintf(stdout, "%f\n", array[index]);
	}
}

void write_double_array_to_file(const char * path, double * array, int array_length)
{
	FILE * fptr = fopen(path, "w");
	int index;

	for(index = 0; index < array_length; index++){
		fprintf(fptr, "%f\n", array[index]);
	}
	fclose(fptr);
}




void interpolating_coefficient_solver(double * y_data, int y_size /* length of y data */, double* output_array)
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


double interp_poly(double * coefficients, double * nodes, double x)
{

	double h = nodes[1]-nodes[0];
	int k = floor( (x - nodes[1])/h)+2; // We add two instead of one because arrays begin their index at 0. In the text we used indexing starting at -1.


	return coefficients[k-2]*pow((2-(x-nodes[k-2])/h),3) + \
		coefficients[k-1]*( 1 + 3*pow( (1-(x-nodes[k-1])/h),1) + 3*pow( (1-(x-nodes[k-1])/h),2) - 3*pow( 1 - (x-nodes[k-1])/h, 3) )+ \
		coefficients[k]*(1+3*pow( 1+ (x-nodes[k])/h, 1) + 3*pow( 1+ (x-nodes[k])/h, 2)-3*pow(1+(x-nodes[k])/h,3))+ \
		coefficients[k+1]*pow(2+(x-nodes[k+1])/h,3);
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

	write_double_array_to_file("colour_parameters/red_values", red_array_float, count); /////////////////////////////// Delete this


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

	double *coefficients_green = malloc((count+2)*sizeof(double));

	double * coefficients_blue = malloc((count+2)*sizeof(double));

	/* interpolating_coefficient_solver is a void function that will output its results into the third argument */

	interpolating_coefficient_solver(red_array_float, count, coefficients_red);

	interpolating_coefficient_solver(green_array_float, count, coefficients_green);

	interpolating_coefficient_solver(blue_array_float, count, coefficients_blue);


	/*Now I want to write these coefficents out into the files */


	write_double_array_to_file("colour_parameters/nodes", nodes, count+2);
	write_double_array_to_file("colour_parameters/red_coefficients", coefficients_red, count+2);
	write_double_array_to_file("colour_parameters/green_coefficients", coefficients_green, count+2);
	write_double_array_to_file("colour_parameters/blue_coefficients", coefficients_blue, count+2);





	//Test the polynomial function

	int num_of_points =100;

	double * image_red = malloc(num_of_points*sizeof(double));
	double * image_green = malloc(num_of_points*sizeof(double));
	double * image_blue = malloc(num_of_points*sizeof(double));
	double * domain = malloc(num_of_points*sizeof(double));

	for( index=0 ; index<num_of_points; index++ )
	{
		domain[index] = 0+ index*( (double)1/(num_of_points-1));
		image_red[index] = interp_poly(coefficients_red, nodes, domain[index]);
		image_green[index]= interp_poly(coefficients_green, nodes, domain[index]);
		image_blue[index] = interp_poly(coefficients_blue, nodes, domain[index]);
	}


	// Write colour data to a csv


	FILE * my_csv_file_pointer =fopen("colour_parameters/colour.csv", "w");


	for( index = 0 ; index < num_of_points ; index++)
	{
		fprintf(my_csv_file_pointer, "%f, %f, %f, %f\n", domain[index], image_red[index], image_green[index], image_blue[index]);
	}

	fclose(my_csv_file_pointer);

	return 0;

}



	//Alright so here is my polynomial. I think it should actually be quite fast because there isn't any loops or logical chains
