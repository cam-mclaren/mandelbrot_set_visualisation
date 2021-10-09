# A makefile for zoom.c 


# My C compiler is 
COMPILER = gcc


SBJS = zoom.c

OBJS = $(SBJS:%.c=%.o)


zoom.exe: $(OBJS) 
	$(COMPILER) $(OBJS) -o zoom.exe

.c.o:
	$(COMPILER) $(MACROS) -c $(SBJS)

clean:
	rm -rf *.o *.exe
test:
	./zoom.exe 10
