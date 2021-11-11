#My complier 

COMPILER = gcc

# Linker Flags 

LINKER_FLAGS = -lmpfr -lgmp 

#Compilation flags 

COMPILER_FLAGS = 

TARGET_FILES = adder.c


OBJECT_FILES = $(TARGET_FILES:%.c=%.o)

main: $(OBJECT_FILES) 
	$(COMPILER) $(LINKER_FLAGS) $(OBJECT_FILES) -o main.exe

compile:
	$(COMPLIER) $(COMPILER_FLAGS) -c $*.c

runmain: main.exe
	./main.exe

clean: 
	rm -rf *.o *.exe




