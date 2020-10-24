IMG = hrbos.img
IPL = ipl10.bin
SYS = hrbos.sys
HED = asmhead.bin
BOTPAK = bootpack.hrb
OBJ = bootpack.o nasmfunc.o graphic.o dsctbl.o int.o hankaku.o hrblib.o fifo.o keyboard.o mouse.o memory.o sheet.o timer.o mtask.o window.o console.o file.o
LD = hrbos.ld
MAP = bootpack.map
C_APP = a.hrb hello3.hrb hello4.hrb hello5.hrb winhelo.hrb winhelo2.hrb winhelo3.hrb star1.hrb stars.hrb stars2.hrb lines.hrb walk.hrb noodle.hrb beepdown.hrb color.hrb color2.hrb 
API = api001.o api002.o api003.o api004.o api005.o api006.o api007.o api008.o api009.o api010.o api011.o api012.o api013.o api014.o api015.o api016.o api017.o api018.o api019.o api020.o
APILIB = libhapi.a

all: $(IMG)

api: $(APILIB)

$(APILIB): $(API)
	ar r $(APILIB) $(API)

$(IMG): $(IPL) $(SYS) $(C_APP) textfile.txt 
	mformat -f 1440 -C -B $< -i $@ ::
	mcopy -i $@ $(SYS) ::
	mcopy -i $@ $(C_APP) ::
	mcopy -i $@ textfile.txt ::

$(IPL): $(IPL:.bin=.asm)
	nasm $^ -o $@

$(SYS): $(HED) $(BOTPAK)
	cat $(HED) $(BOTPAK) > $(SYS)

$(HED): $(HED:.bin=.asm)
	nasm $^ -o $@

$(BOTPAK): $(OBJ) $(LD) 
	ld -m elf_i386 -nostdlib -T $(LD) -Map=$(MAP) $(OBJ)

%.hrb: %.o $(APILIB) hrbapp.ld hrblib.o
	ld -m elf_i386 -nostdlib -Thrbapp.ld -o $@ --no-check-sections $< hrblib.o -L. -lhapi

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
	rm -f $(C_APP)
	rm -f $(APILIB)
