#include "bootpack.h"
#include "hrblib.h"

extern struct KEYBUF keybuf;

void HariMain(void) {
    struct BOOTINFO *binfo = (struct BOOTINFO *) ADR_BOOTINFO;
    char s[40], mcursor[256];
    int mx, my;
    int i, args[2];

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
        io_cli();
        if (keybuf.flag == 0) {
            io_stihlt();
        } else {
            i = keybuf.data;
            keybuf.flag = 0;
            io_sti();
            args[0] = i;
            hrb_sprintfi(s, "%2X", args);
            boxfill8(binfo->vram, binfo->scrnx, COL8_008484, 0, 16, 15, 31);
            putfonts8_asc(binfo->vram, binfo->scrnx, 0, 16, COL8_FFFFFF, s);
        }
    }
}


