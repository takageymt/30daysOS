#include "hrblib.h"

extern void io_hlt(void);
extern void io_cli(void);
extern void io_out8(int port, int data);
extern int io_load_eflags(void);
extern void io_store_eflags(int eflags);

void init_palette(void);
void set_palette(int start, int end, unsigned char *rgb);
void boxfill8(unsigned char *vram, int xsize, unsigned char c, int x0, int y0, int x1, int y1);
void init_screen(char *vram, int xsize, int ysize);
void putfont8(char *vram, int xsize, int x, int y, char c, char *font);
void putfonts8_asc(char *vram, int xsize, int x, int y, char c, unsigned char *s);

#define COL8_000000 0
#define COL8_FF0000 1
#define COL8_00FF00 2
#define COL8_FFFF00 3
#define COL8_0000FF 4
#define COL8_FF00FF 5
#define COL8_00FFFF 6
#define COL8_FFFFFF 7
#define COL8_C6C6C6 8
#define COL8_840000 9
#define COL8_008400 10
#define COL8_848400 11
#define COL8_000084 12
#define COL8_840084 13
#define COL8_008484 14
#define COL8_848484 15

struct BOOTINFO {
    char cyls, leds,vmode, reserve;
    short scrnx, scrny;
    char *vram;
};

void HariMain(void) {
    struct BOOTINFO *binfo = (struct BOOTINFO *) 0x0ff0;
    int args[1];
    char s[40];

    init_palette();
    init_screen(binfo->vram, binfo->scrnx, binfo->scrny);

    putfonts8_asc(binfo->vram, binfo->scrnx,  8,  8, COL8_FFFFFF, "ABC 123");
    putfonts8_asc(binfo->vram, binfo->scrnx, 31, 31, COL8_000000, "Haribote OS.");
    putfonts8_asc(binfo->vram, binfo->scrnx, 30, 30, COL8_FFFFFF, "Haribote OS.");

    args[0] = binfo->scrnx;
    hrb_sprintfi(s, "scrnx = %d", args);
    putfonts8_asc(binfo->vram, binfo->scrnx, 16, 64, COL8_FFFFFF, s);
    
    for (;;) {
        io_hlt();
    }
}

void init_palette(void) {
    static unsigned char table_rgb[16*3] = {
        0x00, 0x00, 0x00,   //  0: black
        0xff, 0x00, 0x00,   //  1: light red
        0x00, 0xff, 0x00,   //  2: light green
        0xff, 0xff, 0x00,   //  3: light yellow
        0x00, 0x00, 0xff,   //  4: light blue
        0xff, 0x00, 0xff,   //  5: light purple
        0x00, 0xff, 0xff,   //  6: light sky
        0xff, 0xff, 0xff,   //  7: white
        0xc6, 0xc6, 0xc6,   //  8: light gray
        0x84, 0x00, 0x00,   //  9: dark red
        0x00, 0x84, 0x00,   // 10: dark green
        0x84, 0x84, 0x00,   // 11: dark yellow
        0x00, 0x00, 0x84,   // 12: dark blue
        0x84, 0x00, 0x84,   // 13: dark purple
        0x00, 0x84, 0x84,   // 14: dark sky
        0x84, 0x84, 0x84    // 15: dark gray
    };
    set_palette(0, 15, table_rgb);
    return;
}

void set_palette(int start, int end, unsigned char *rgb) {  
    int i, eflags;
    
    eflags = io_load_eflags();  // save interrupt enable flag
    io_cli();   // clear interrupt flag; to disable interrupt 
    io_out8(0x03c8, start);
    for (i = start; i <= end; i++) {
        io_out8(0x03c9, rgb[0] / 4);
        io_out8(0x03c9, rgb[1] / 4);
        io_out8(0x03c9, rgb[2] / 4);
        rgb += 3;
    }
    io_store_eflags(eflags);    // set interrupt flag; to enable interrupt
    return;
}

void boxfill8(unsigned char *vram, int xsize, unsigned char c, int x0, int y0, int x1, int y1) {
    int x, y;
    for (y = y0; y <= y1; y++) {
        for (x = x0; x <= x1; x++) {
            vram[y * xsize + x] = c;
        }
    }
    return;
}

void init_screen(char *vram, int xsize, int ysize) {
    boxfill8(vram, xsize, COL8_008484, 0,        0, xsize-1, ysize-29);
    boxfill8(vram, xsize, COL8_C6C6C6, 0, ysize-28, xsize-1, ysize-28);
    boxfill8(vram, xsize, COL8_FFFFFF, 0, ysize-27, xsize-1, ysize-27);
    boxfill8(vram, xsize, COL8_C6C6C6, 0, ysize-26, xsize-1, ysize- 1);

    boxfill8(vram, xsize, COL8_FFFFFF,  3, ysize-24, 59, ysize-24);
    boxfill8(vram, xsize, COL8_FFFFFF,  2, ysize-24,  2, ysize- 4);
    boxfill8(vram, xsize, COL8_848484,  3, ysize- 4, 59, ysize- 4);
    boxfill8(vram, xsize, COL8_848484, 59, ysize-23, 59, ysize- 5);
    boxfill8(vram, xsize, COL8_000000,  2, ysize- 3, 59, ysize- 3);
    boxfill8(vram, xsize, COL8_000000, 60, ysize-24, 60, ysize- 3);

    
    boxfill8(vram, xsize, COL8_848484, xsize-47, ysize-24, xsize- 4, ysize-24);
    boxfill8(vram, xsize, COL8_848484, xsize-47, ysize-23, xsize-47, ysize- 4);
    boxfill8(vram, xsize, COL8_FFFFFF, xsize-47, ysize- 3, xsize- 4, ysize- 3);
    boxfill8(vram, xsize, COL8_FFFFFF, xsize- 3, ysize-24, xsize- 3, ysize- 3);

    return;
}

void putfont8(char *vram, int xsize, int x, int y, char c, char *font) {
    int i;
    char *p, d;
    for (i = 0; i < 16; i++) {
        p = vram + (y+i)*xsize + x;
        d = font[i];
        if (d&0x80) p[0] = c;
        if (d&0x40) p[1] = c;
        if (d&0x20) p[2] = c;
        if (d&0x10) p[3] = c;
        if (d&0x08) p[4] = c;
        if (d&0x04) p[5] = c;
        if (d&0x02) p[6] = c;
        if (d&0x01) p[7] = c;
    }
    return;
}

void putfonts8_asc(char *vram, int xsize, int x, int y, char c, unsigned char *s) {
    extern char hankaku[4096];
    for (; *s != 0x00; s++) {
        putfont8(vram, xsize, x, y, c, hankaku + *s * 16);
        x += 8;
    }
    return;
}
