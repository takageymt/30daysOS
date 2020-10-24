LIB=libhutil.a
OBJ=_hrb_rand.o _hrb_strcpy.o _hrb_strcmp.o _hrb_sprintfi.o

all: $(LIB)

$(LIB): $(OBJ)
	ar r $@ $^

%.o: %.c
	gcc -m32 -nostdlib -fno-pic -c $< -o $@ 

clean:
	rm -f $(LIB)
	rm -f *.o
