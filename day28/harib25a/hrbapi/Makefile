LIB=libhapi.a
OBJ=api001.o api002.o api003.o api004.o api005.o api006.o api007.o api008.o api009.o api010.o api011.o api012.o api013.o api014.o api015.o api016.o api017.o api018.o api019.o api020.o api050.o

all: $(LIB)

$(LIB): $(OBJ)
	ar r $@ $^

%.o: %.asm
	nasm -f elf32 -o $@ $<

clean:
	rm -f $(LIB)
	rm -f *.o
