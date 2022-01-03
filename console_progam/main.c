#include <stdio.h>
#include <stdlib.h>







int print_main_menu();
int menu_item_select(int option_number);
int take_int_input(void);



int main(void/* I don't think this needs inputs from stdarg */ ){


// print a welcome to mandelbrot set visualiser
// I can do this later
// Check directory?

// Print main menu
int test_int;
print_main_menu();
test_int=take_int_input();
printf("%d\n", test_int);




return 0;
}






int print_main_menu()
{
  fprintf(stdout,"\nMAIN MENU\n");
  int i, bar_width=30;
  for(i=0; i<bar_width; i++)
  {
    fprintf(stdout,"=");
  }
  printf("\n");

  printf("1. Choose zoom point\n\
2. Choose zoom maginfication\n\
3. Create zoom path file\n\
4. Choose zoom path file\n\
5. Choose image output folder\n\
6. Generate zoom images\n");

printf("\nSelect an above option number\n");
  return 0;
}

int menu_item_select(int option_number){
 return 0;
}


int take_int_input(void)
{
  char  * string_input = malloc(100*sizeof(char));
  scanf("%s", string_input);

  int integer;

  integer=atoi(string_input);

  if(integer==0)
  {printf("Warning: atoi() returned 0. take_int_input() cannot read \"0\".");}

  return integer;
}
