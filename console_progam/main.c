#include <stdio.h>
#include <stdlib.h>
#include <string.h>



struct menu {
  char title[50];
  char options[500];
  int option_number;
};



int take_int_input(void);
int run_menu(struct menu menu_name);



int main(void/* I don't think this needs inputs from stdarg */ ){


// print a welcome to mandelbrot set visualiser
// I can do this later
// Check directory?
int program_exit_status=0;

struct menu main_menu;
strcpy(main_menu.title, "MAIN MENU\n");
strcpy(main_menu.options,\
  "1. Choose zoom point\n\
2. Choose zoom maginfication\n\
3. Create zoom path file\n\
4. Choose zoom path file\n\
5. Choose image output folder\n\
6. Generate zoom images\n\
7. Print manual\n\
8. Close program\n"
);
main_menu.option_number=8;


int main_menu_selection;

while(program_exit_status==0)
{

  main_menu_selection=run_menu(main_menu);
  printf("%d\n", main_menu_selection);

  if(main_menu_selection==8) //close program
  {
    program_exit_status=1;
  }
}

return 0;
}


int take_int_input(void)
{
  char  * string_input = malloc(100*sizeof(char));
  scanf("%s", string_input);
  int integer;
  integer=atoi(string_input);
  if(integer==0)
  {printf("Warning: atoi() returned 0. take_int_input() cannot read \"0\".\n");}
  return integer;
}



int run_menu(struct menu menu_name)
{
  int selection;
  int exit_status=0;
  while (exit_status==0)
  {
    fprintf(stdout,"%s", menu_name.title);
    int i, bar_width=30;
    for(i=0; i<bar_width; i++)
    {
      fprintf(stdout,"=");
    }
    printf("\n");

    printf("%s",menu_name.options);
    printf("\nSelect an above option number\n");

    printf("option choice:");

    selection = take_int_input();

    if((selection>menu_name.option_number)||(selection<1))
    {
      printf("Please select a valid option.\n");
      //scanf(" ");
    } else
    {
      exit_status=1;
    }
  }
  return selection;
}
