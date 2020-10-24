IMG = hrbos.img
IPL = ipl10.bin
SYS = hrbos.sys
HED = asmhead.bin
BOTPAK = bootpack.hrb
OBJ = bootpack.o nasmfunc.o graphic.o dsctbl.o int.o hankaku.o hrblib.o fifo.o keyboard.o mouse.o memory.o sheet.o timer.o mtask.o window.o console.o file.o
LD = hrbos.ld
MAP = bootpack.map

all: $(IMG)

$(IMG): $(IPL) $(SYS) textfile.txt hlt.hrb
	mformat -f 1440 -C -B $< -i $@ ::
	mcopy -i $@ $(SYS) ::
	mcopy -i $@ textfile.txt ::
	mcopy -i $@ hlt.hrb ::

$(IPL): $(IPL:.bin=.asm)
	nasm $^ -o $@

$(SYS): $(HED) $(BOTPAK)
	cat $(HED) $(BOTPAK) > $(SYS)

$(HED): $(HED:.bin=.asm)
	nasm $^ -o $@

$(BOTPAK): $(OBJ) $(LD) 
	ld -m elf_i386 -nostdlib -T $(LD) -Map=$(MAP) $(OBJ)

nasmfunc.o: nasmfunc.asm
	nasm -f elf32 -o $@ $<

hlt.hrb: hlt.asm
	nasm -f elf32 -o $@ $<

%.o: %.c
	gcc -m32 -nostdlib -fno-pic -c $< -o $@ # -nostdlib is not neccesary

run: $(IMG)
	qemu-system-i386 -m size=32 -icount shift=1 -fda $<

debug: $(IMG)
	qemu-system-i386 -m size=32 -localtime -vga std -icount shift=1 -fda $< -gdb tcp::10000 -S &

clean:
	rm -f $(IMG) $(IPL) $(SYS) $(BOTPAK)
	rm -f *.bin 
	rm -f *.o
	rm -f hlt.hrb
