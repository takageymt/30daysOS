IMG = helloos.img
IPL = ipl.bin

all: $(IMG)

$(IMG): $(IPL)
	mformat -f 1440 -C -B $< -i $@ ::

$(IPL): $(IPL:.bin=.asm)
	nasm $^ -o $@

run: $(IMG)
	qemu-system-i386 -fda $<

clean:
	rm -f $(IMG) $(IPL)
