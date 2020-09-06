IMG = hrbos.img
IPL = ipl10.bin
SYS = hrbos.sys
HRB = bootpack.hrb
HED = asmhead.bin
BOTPAK = bootpack.bin
OBJ = bootpack.o
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
	ld -m elf_i386 -T $(LD) -o $@ $(OBJ)

$(OBJ): $(BOTPAK:.bin=.c)
	gcc -m32 -fno-pic -c $< -o $@

run: $(IMG)
	qemu-system-i386 -fda $<

clean:
	rm -f $(IMG) $(IPL) $(SYS)
	rm -f *.bin 
	rm -f *.o
