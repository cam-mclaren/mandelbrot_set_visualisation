#My complier 

COMPILER = gcc

# Linker Flags 

LINKER_FLAGS = -lmpfr -lgmp 

#Compilation flags 

COMPILER_FLAGS = 

TARGET_FILES = single_image.c

SECONDARY_TARGET = write_coordinates.c

OBJECT_FILES = $(TARGET_FILES:%.c=%.o)

SECONDARY_OBJECT_FILES=$(SECONDARY_TARGET:%.c=%.o) 

main.out: $(OBJECT_FILES) 
	$(COMPILER)  $(OBJECT_FILES)  $(LINKER_FLAGS) -o main.out

compile:
	$(COMPILER) $(COMPILER_FLAGS) -c $(TARGET_FILES)

runmain: main.out
	chmod 777 ../sockets/socket.sock && chmod 777 main.out && ./main.out $$(cat image_parameters.txt | awk '{printf "%50.50s ", $$0}')

clean: 
	rm -rf *.o *.out


aux: $(SECONDARY_OBJECT_FILES)
	$(COMPILER) $(LINKER_FLAGS) $(SECONDARY_OBJECT_FILES) -o aux.out 

aux_compile:
	$(COMPILER) $(COMPILER_FLAGS) -c $(SECONDARY_TARGET)

nextcentre:
	$(COMPILER)  nextcentre.c $(LINKER_FLAGS) -o next.out
