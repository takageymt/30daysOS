IMG = hrbos.img
IPL = ipl10.bin
SYS = haribote.sys

all: $(IMG)

$(IMG): $(IPL) $(SYS)
	mformat -f 1440 -C -B $< -i $@ ::
	mcopy -i $@ $(SYS) ::

$(IPL): $(IPL:.bin=.asm)
	nasm $^ -o $@

$(SYS): $(SYS:.sys=.asm)
	nasm $^ -o $@

run: $(IMG)
	qemu-system-i386 -fda $<

clean:
	rm -f $(IMG) $(IPL) $(SYS)
