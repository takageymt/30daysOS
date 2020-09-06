IMG = hrbos.img
IPL = ipl10.bin
SYS = hrbos.sys
HED = asmhead.bin
BOTPAK = bootpack.hrb
OBJ = bootpack.o nasmfunc.o
LD = hrbos.ld

all: $(IMG)

$(IMG): $(IPL) $(SYS)
	mformat -f 1440 -C -B $< -i $@ ::
	mcopy -i $@ $(SYS) ::

$(IPL): $(IPL:.bin=.asm)
	nasm $^ -o $@

$(SYS): $(HED) $(BOTPAK)
	cat $(HED) $(BOTPAK) > $(SYS)

$(HED): $(HED:.bin=.asm)
	nasm $^ -o $@

$(BOTPAK): $(OBJ) $(LD)
	ld -m elf_i386 -T $(LD)

bootpack.o: bootpack.c
	gcc -m32 -fno-pic -c $< -o $@

nasmfunc.o: nasmfunc.asm
	nasm -f elf32 -o $@ $<

run: $(IMG)
	qemu-system-i386 -fda $<

clean:
	rm -f $(IMG) $(IPL) $(SYS)
	rm -f *.bin 
	rm -f *.o
