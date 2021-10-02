# A makefile for the colours


# My C compiler is
COMPILER = gcc

# A list of compiler flags

#List of textfiles to compile
SBJS = polynomial_function.c

# The list of object files
OBJS = $(SBJS:%.c=%.o)

main.exe: $(OBJS)
	$(COMPILER) $(OBJS) -o main.exe

.c.o:
	$(COMPILER) -c $(SBJS)

clean:
	rm -rf *.o *.exe

test: 
	./main.exe hex_file
