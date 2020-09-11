#include "bootpack.h"
#include "hrblib.h"

void HariMain(void) {
    struct BOOTINFO *binfo = (struct BOOTINFO *) ADR_BOOTINFO;
    char s[40], mcursor[256];
    int mx, my;
    int args[2];

    init_gdtidt();
    init_pic();
    io_sti(); // set interrupt flag as end of IDT/PIC initialization

    init_palette();
    init_screen(binfo->vram, binfo->scrnx, binfo->scrny);

    mx = (binfo->scrnx-16) / 2;
    my = (binfo->scrny-28-16) / 2;
    init_mouse_cursor8(mcursor, COL8_008484);
    putblock8_8(binfo->vram, binfo->scrnx, 16, 16, mx, my, mcursor, 16);

    args[0] = mx;
    args[1] = my;
    hrb_sprintfi(s, "(%d, %d)", args);
    putfonts8_asc(binfo->vram, binfo->scrnx, 0, 0, COL8_FFFFFF, s);

    io_out8(PIC0_IMR, 0xf9); // enable int. from PIC1 and a keyboard (11111001)
    io_out8(PIC1_IMR, 0xef); // enable int. from a mouse (0x11101111)
    
    for (;;) {
        io_hlt();
    }
}


