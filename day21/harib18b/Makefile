IMG = hrbos.img
IPL = ipl10.bin
SYS = hrbos.sys
HED = asmhead.bin
BOTPAK = bootpack.hrb
OBJ = bootpack.o nasmfunc.o graphic.o dsctbl.o int.o hankaku.o hrblib.o fifo.o keyboard.o mouse.o memory.o sheet.o timer.o mtask.o window.o console.o file.o
LD = hrbos.ld
MAP = bootpack.map
APP = hello.hrb hello2.hrb a.hrb hello3.hrb

all: $(IMG)

$(IMG): $(IPL) $(SYS) $(APP) textfile.txt 
	mformat -f 1440 -C -B $< -i $@ ::
	mcopy -i $@ $(SYS) ::
	mcopy -i $@ $(APP) ::
	mcopy -i $@ textfile.txt ::

$(IPL): $(IPL:.bin=.asm)
	nasm $^ -o $@

$(SYS): $(HED) $(BOTPAK)
	cat $(HED) $(BOTPAK) > $(SYS)

$(HED): $(HED:.bin=.asm)
	nasm $^ -o $@

$(BOTPAK): $(OBJ) $(LD) 
	ld -m elf_i386 -nostdlib -T $(LD) -Map=$(MAP) $(OBJ)

hello.hrb: hello.asm
	nasm -f bin -o $@ $<
hello2.hrb: hello2.asm
	nasm -f bin -o $@ $<

%.hrb: %.o a_nasm.o hrbapp.ld
	ld -m elf_i386 -nostdlib -Thrbapp.ld -o $@ $< a_nasm.o

%.o: %.asm
	nasm -f elf32 -o $@ $<

%.o: %.c
	gcc -m32 -nostdlib -fno-pic -c $< -o $@ # -nostdlib is not neccesary

run: $(IMG)
	qemu-system-i386 -m size=32 -icount shift=1 -fda $<

clean:
	rm -f $(IMG) $(IPL) $(SYS) $(BOTPAK)
	rm -f *.bin 
	rm -f *.o
	rm -f $(APP)
