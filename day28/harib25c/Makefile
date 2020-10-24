IMG = hrbos.img
IPL = ipl10.bin
SYS = hrbos.sys
HED = asmhead.bin
BOTPAK = bootpack.hrb
OBJ = bootpack.o nasmfunc.o graphic.o dsctbl.o int.o hankaku.o fifo.o keyboard.o mouse.o memory.o sheet.o timer.o mtask.o window.o console.o file.o
LD = hrbos.ld
MAP = bootpack.map
C_APP = a.hrb hello3.hrb hello4.hrb hello5.hrb winhelo.hrb winhelo2.hrb winhelo3.hrb star1.hrb stars.hrb stars2.hrb lines.hrb walk.hrb noodle.hrb beepdown.hrb color.hrb color2.hrb prime.hrb catipl.hrb

all: $(IMG)

$(IMG): $(IPL) $(SYS) $(C_APP) textfile.txt 
	mformat -f 1440 -C -B $< -i $@ ::
	mcopy -i $@ $(SYS) ::
	mcopy -i $@ $(C_APP) ::
	mcopy -i $@ textfile.txt ::
	mcopy -i $@ ipl10.asm ::
    
$(IPL): $(IPL:.bin=.asm)
	nasm $^ -o $@

$(SYS): $(HED) $(BOTPAK)
	cat $(HED) $(BOTPAK) > $(SYS)

$(HED): $(HED:.bin=.asm)
	nasm $^ -o $@

$(BOTPAK): $(OBJ) $(LD) 
	ld -m elf_i386 -nostdlib -T $(LD) -Map=$(MAP) $(OBJ) -L. -lhutil

$(ASM_APP): $(@:.hrb=.asm)
	nasm -f bin -o $@ $(@:.hrb=.asm)

%.hrb: %.o hrbapp.ld 
	ld -m elf_i386 -nostdlib -Thrbapp.ld -o $@ --no-check-sections $< -L./hrbapi -lhapi -L./hrbutil -lhutil -Map=$(@:.hrb=.map)

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
	rm -f *.map
