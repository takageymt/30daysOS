#include "bootpack.h"
#include "hrblib.h"

void init_pic(void) {
    io_out8(PIC0_IMR, 0xff); // do not receive any int.
    io_out8(PIC1_IMR, 0xff); // do not receive any int.

    io_out8(PIC0_ICW1, 0x11); // edge trigger mode
    io_out8(PIC0_ICW2, 0x20); // IRQ0-7 by INT20-27 
    io_out8(PIC0_ICW3, 1<<2); // connect to PIC1 with IRQ2
    io_out8(PIC0_ICW4, 0x01); // non-buffered mode

    io_out8(PIC1_ICW1, 0x11); // edge trigger mode
    io_out8(PIC1_ICW2, 0x28); // IRQ8-15 by INT28-2f
    io_out8(PIC1_ICW3, 2   ); // connect to PIC0 with IQR2
    io_out8(PIC1_ICW4, 0x01); // non-buffered mode
    
    io_out8(PIC0_IMR, 0xfb); // 11111011 receive only from PIC1
    io_out8(PIC1_IMR, 0xff); // 11111111 do not receive any int.

    return;
}

# define PORT_KEYDAT    0x0060

struct KEYBUF keybuf;

void inthandler21(int *esp) {
    unsigned char data;
    io_out8(PIC0_OCW2, 0x61); // notify completing to receive IRQ01
    data = io_in8(PORT_KEYDAT);

    if (keybuf.flag == 0) {
        keybuf.data = data;
        keybuf.flag = 1;
    }

    return;
}

void inthandler2c(int *esp) {
    struct BOOTINFO *binfo = (struct BOOTINFO *) ADR_BOOTINFO;
    boxfill8(binfo->vram, binfo->scrnx, COL8_000000, 0, 0, 32*8 - 1, 15);
    putfonts8_asc(binfo->vram, binfo->scrnx, 0, 0, COL8_FFFFFF, "INT 2C (IRQ-12) : PS/2 mouse");
    for (;;) {
        io_hlt();
    }
}

void inthandler27(int *esp) {
    io_out8(PIC0_OCW2, 0x67);
    return;
}
