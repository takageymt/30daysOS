IMG = hrbos.img
IPL = ipl10.bin
SYS = hrbos.sys
HED = asmhead.bin
BOTPAK = bootpack.hrb
OBJ = bootpack.o nasmfunc.o graphic.o dsctbl.o int.o hankaku.o hrblib.o
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
	ld -m elf_i386 -nostdlib -T $(LD) $(OBJ)

nasmfunc.o: nasmfunc.asm
	nasm -f elf32 -o $@ $<

%.o: %.c
	gcc -m32 -nostdlib -fno-pic -c $< -o $@ # -nostdlib is not neccesary

run: $(IMG)
	qemu-system-i386 -fda $<

clean:
	rm -f $(IMG) $(IPL) $(SYS) $(BOTPAK)
	rm -f *.bin 
	rm -f *.o
