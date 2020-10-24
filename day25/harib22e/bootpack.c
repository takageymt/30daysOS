#include "bootpack.h"
#include "hrblib.h"

#define KEYCMD_LED  0xed
#define NUM_CONS    2

int keywin_off(struct SHEET *key_win, struct SHEET *sht_win, int cur_c, int cur_x);
int keywin_on(struct SHEET *key_win, struct SHEET *sht_win, int cur_c);

void HariMain(void) {
    struct BOOTINFO *binfo = (struct BOOTINFO *) ADR_BOOTINFO;
    struct SHTCTL *shtctl;
    char s[40];
    struct FIFO32 fifo, keycmd;
    int fifobuf[128], keycmd_buf[32];
    int mx, my, i, cursor_x, cursor_c;
    unsigned int memtotal;
    struct MOUSE_DEC mdec;
    struct MEMMAN *memman = (struct MEMMAN *) MEMMAN_ADDR;
    unsigned char *buf_back, buf_mouse[256], *buf_win, *buf_cons[NUM_CONS];
    struct SHEET *sht_back, *sht_mouse, *sht_win, *sht_cons[NUM_CONS];
    struct TASK *task_a, *task_cons[NUM_CONS], *task;
    struct TIMER *timer;
    static char keytable0[0x80] = {
          0,   0, '1', '2', '3', '4', '5', '6',  '7', '8', '9',  '0', '-',  '=',   0,   0,
        'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I',  'O', 'P', '[',  ']',   0,    0, 'A', 'S',
        'D', 'F', 'G', 'H', 'J', 'K', 'L', ';', '\'', '`',   0, '\\', 'Z',  'X', 'C', 'V',
        'B', 'N', 'M', ',', '.', '/',   0, '*',    0, ' ',   0,    0,   0,    0,   0,   0,
          0,   0,   0,   0,   0,   0,   0, '7',  '8', '9', '-',  '4', '5',  '6', '+', '1',
        '2', '3', '0', '.',   0,   0,   0,   0,    0,   0,   0,    0,   0,    0,   0,   0,
          0,   0,   0,   0,   0,   0,   0,   0,    0,   0,   0,    0,   0,    0,   0,   0,
          0,   0,   0, '`',   0,   0,   0,   0,    0,   0,   0,    0,   0, '\\',   0,   0
    };
    static char keytable1[0x80] = {
          0,   0, '!', '@', '#', '$', '%', '^',  '&', '*', '(',  ')', '_',  '+',   0,   0,
        'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I',  'O', 'P', '{',  '}',   0,    0, 'A', 'S',
        'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '\"', '~',   0,  '|', 'Z',  'X', 'C', 'V',
        'B', 'N', 'M', '<', '>', '?',   0, '*',    0, ' ',   0,    0,   0,    0,   0,   0,
          0,   0,   0,   0,   0,   0,   0, '7',  '8', '9', '-',  '4', '5',  '6', '+', '1',
        '2', '3', '0', '.',   0,   0,   0,   0,    0,   0,   0,    0,   0,    0,   0,   0,
          0,   0,   0,   0,   0,   0,   0,   0,    0,   0,   0,    0,   0,    0,   0,   0,
          0,   0,   0, '~',   0,   0,   0,   0,    0,   0,   0,    0,   0,  '|',   0,   0
    };
    int key_shift = 0, key_ctrl = 0, key_alt = 0, key_leds = (binfo->leds >> 4) & 7, keycmd_wait = -1;
    int j, x, y, mmx = -1, mmy = -1;
    struct SHEET *sht = 0, *key_win;
    int args[10];

    // At first, enable interrupt
    init_gdtidt();
    init_pic();
    io_sti(); // set interrupt flag as the end of IDT/PIC initialization

    fifo32_init(&fifo, 128, fifobuf, 0);
    init_pit();
    init_keyboard(&fifo, 256);
    enable_mouse(&fifo, 512, &mdec);
    io_out8(PIC0_IMR, 0xf8); // enable int. from PIC1, a timer, and keyboard (11111000)
    io_out8(PIC1_IMR, 0xef); // enable int. from a mouse (0x11101111)

    memtotal = memtest(0x00400000, 0xbfffffff);
    memman_init(memman);
    memman_free(memman, 0x00001000, 0x0009e000); 
    memman_free(memman, 0x00400000, memtotal - 0x00400000);

    init_palette();
    shtctl = shtctl_init(memman, binfo->vram, binfo->scrnx, binfo->scrny);
    task_a = task_init(memman);
    fifo.task = task_a;
    task_run(task_a, 1, 2);
    *((int *) 0x0fe4) = (int) shtctl;

    // sht_back
    sht_back  = sheet_alloc(shtctl);
    buf_back  = (unsigned char *) memman_alloc_4k(memman, binfo->scrnx * binfo->scrny);
    sheet_setbuf(sht_back, buf_back, binfo->scrnx, binfo->scrny, -1);
    init_screen8(buf_back, binfo->scrnx, binfo->scrny);

    // sht_cons
    for (i = 0; i < NUM_CONS; i++) {
        sht_cons[i] = sheet_alloc(shtctl);
        buf_cons[i] = (unsigned char *) memman_alloc_4k(memman, 256 * 165);
        sheet_setbuf(sht_cons[i], buf_cons[i], 256, 165, -1);
        make_window8(buf_cons[i], 256, 165, "console", 0);
        make_textbox8(sht_cons[i], 8, 28, 240, 128, COL8_000000);

        task_cons[i] = task_alloc();
        task_cons[i]->tss.esp = memman_alloc_4k(memman, 64*1024) + 64*1024 - 12; // SP to the end of a stack
        task_cons[i]->tss.eip = (int) &console_task; // entry point of task_b (4); IP = instruction pointer
        task_cons[i]->tss.es = 1*8;
        task_cons[i]->tss.cs = 2*8; // GDT 2 (this OS program)
        task_cons[i]->tss.ss = 1*8;
        task_cons[i]->tss.ds = 1*8;
        task_cons[i]->tss.fs = 1*8;
        task_cons[i]->tss.gs = 1*8;
        *((int *) (task_cons[i]->tss.esp + 4)) = (int) sht_cons[i];
        *((int *) (task_cons[i]->tss.esp + 8)) = memtotal;
        task_run(task_cons[i], 2, 2);
        sht_cons[i]->task = task_cons[i];
        sht_cons[i]->flags |= 0x20;
    }

    // sht_win
    sht_win   = sheet_alloc(shtctl);
    buf_win   = (unsigned char *) memman_alloc_4k(memman, 144 * 52);
    sheet_setbuf(sht_win, buf_win, 144, 52, -1);
    make_window8(buf_win, 144, 52, "task_a", 1);
    make_textbox8(sht_win, 8, 28, 128, 16, COL8_FFFFFF);

    cursor_x = 8;
    cursor_c = COL8_FFFFFF;

    timer = timer_alloc();
    timer_init(timer, &fifo, 1);
    timer_settime(timer, 50);

    // sht_mouse
    sht_mouse = sheet_alloc(shtctl);
    sheet_setbuf(sht_mouse, buf_mouse, 16, 16, 99);
    init_mouse_cursor8(buf_mouse, 99);
    mx = (binfo->scrnx-16) / 2;
    my = (binfo->scrny-28-16) / 2;

    sheet_slide(sht_back,  0,  0);
    sheet_slide(sht_cons[0], 56, 6);
    sheet_slide(sht_cons[1],  8, 2);
    //sheet_slide(sht_win,  64, 56);
    sheet_slide(sht_win,  150, 0);
    sheet_slide(sht_mouse, mx, my);
    sheet_updown(sht_back,  0);
    sheet_updown(sht_cons[0], 1);
    sheet_updown(sht_cons[1], 2);
    sheet_updown(sht_win,     3);
    sheet_updown(sht_mouse,   4);
    key_win = sht_win;

    fifo32_put(&keycmd, KEYCMD_LED);
    fifo32_put(&keycmd, key_leds);

    for (;;) {
        if (fifo32_status(&keycmd) > 0 && keycmd_wait < 0) {
            keycmd_wait = fifo32_get(&keycmd);
            wait_KBC_sendready();
            io_out8(PORT_KEYDAT, keycmd_wait);
        }
        io_cli();
        if (fifo32_status(&fifo) == 0) {
            task_sleep(task_a);
            io_sti();
        } else {
            i = fifo32_get(&fifo);
            io_sti();
            if (key_win->flags == 0) {
                key_win = shtctl->sheets[shtctl->top-1];
                cursor_c = keywin_on(key_win, sht_win, cursor_c);
            }
            if (256 <= i && i <= 511) { // from a keyboard
                args[0] = i-256;
                args[1] = task_cons[0]->tss.ss0;
                hrb_sprintfi(s, "%02X%08X", args);
                putfonts8_asc_sht(sht_win, 8, 28, COL8_000000, COL8_FFFFFF, s, 10);
                /****************
                 *   keyboard   *
                 ****************/
                if (i < 256 + 0x54) {
                    if(key_shift == 0) {
                        s[0] = keytable0[i-256];
                    } else {
                        s[0] = keytable1[i-256];
                    }
                } else {
                    s[0] = 0;
                }
                if ('A' <= s[0] && s[0] <= 'Z') {
                    if (((key_leds&4) == 0 && key_shift == 0) || 
                        ((key_leds&4) != 0 && key_shift != 0)) {
                        s[0] += 0x20;
                    }
                }
                if (s[0] != 0) {
                    if (key_win == sht_win) {
                        if (cursor_x < 128) { // normal characters
                            s[1] = 0;
                            putfonts8_asc_sht(sht_win, cursor_x, 28, COL8_000000, COL8_FFFFFF, s, 1);
                            cursor_x += 8;
                        }
                    } else if (key_ctrl != 0 && i == 256 + 0x2e) {
                        task = key_win->task;
                        if (task != 0 && task->tss.ss0 != 0) {
                            cons_putstr0(task->cons, "\nBreak(key) :\n");
                            io_cli();
                            task->tss.eax = (int) &(task->tss.esp0);
                            task->tss.eip = (int) asm_end_app;
                            io_sti();
                        }
                    } else {
                        fifo32_put(&key_win->task->fifo, s[0] + 256);
                    }
                }
                if (i == 256 + 0x0e) { // backspace
                    if (key_win == sht_win) {
                        if (cursor_x > 8) {
                            putfonts8_asc_sht(sht_win, cursor_x, 28, COL8_000000, COL8_FFFFFF, " ", 1);
                            cursor_x -= 8;
                        }
                    } else {
                        fifo32_put(&key_win->task->fifo, 8 + 256);
                    }
                }
                if (i == 256 + 0x1c) {
                    if (key_win != sht_win) {
                        fifo32_put(&key_win->task->fifo, 10 + 256);
                    }
                }
                if (i == 256 + 0x0f) { // Tab
                    if (key_shift != 0 && shtctl->top > 2) {
                        sheet_updown(shtctl->sheets[1], shtctl->top-1);
                    } else {
                        cursor_c = keywin_off(key_win, sht_win, cursor_c, cursor_x);
                        j = key_win->height - 1;
                        if (j == 0) {
                            j = shtctl->top - 1;
                        }
                        key_win = shtctl->sheets[j];
                        cursor_c = keywin_on(key_win, sht_win, cursor_c);
                    }
                }
                if (i == 256 + 0x1d) {
                    key_ctrl |= 1;
                }
                if (i == 256 + 0x2a) {
                    key_shift |= 1;
                }
                if (i == 256 + 0x36) {
                    key_shift |= 2;
                }
                if (i == 256 + 0x38) {
                    key_alt |= 1;
                }
                if (i == 256 + 0x9d) {
                    key_ctrl &= ~1;
                }
                if (i == 256 + 0xaa) {
                    key_shift &= ~1;
                }
                if (i == 256 + 0xb6) {
                    key_shift &= ~2;
                }
                if (i == 256 + 0xb8) {
                    key_alt &= ~1;
                }
                if (i == 256 + 0x3a) { // CapsLock
                    key_leds ^= 4;
                    fifo32_put(&keycmd, KEYCMD_LED);
                    fifo32_put(&keycmd, key_leds);
                }
                if (i == 256 + 0x45) { // NumLock
                    key_leds ^= 2;
                    fifo32_put(&keycmd, KEYCMD_LED);
                    fifo32_put(&keycmd, key_leds);
                }
                if (i == 256 + 0x46) { // ScrollLock
                    key_leds ^= 1;
                    fifo32_put(&keycmd, KEYCMD_LED);
                    fifo32_put(&keycmd, key_leds);
                }
                if (i == 256 + 0xfa) { // a keyboard has received a cmd successfully
                    keycmd_wait = -1;
                }
                if (i == 256 + 0xfe) { // keyboard cmd error
                    wait_KBC_sendready();
                    io_out8(PORT_KEYDAT, keycmd_wait);
                }
                if (cursor_c >= 0) {
                    boxfill8(sht_win->buf, sht_win->bxsize, cursor_c, cursor_x, 28, cursor_x + 7, 43);
                }
                sheet_refresh(sht_win, cursor_x, 28, cursor_x + 8, 44);
            } else if (512 <= i && i <= 767) { // from a mouse
                /*************
                 *   mouse   *
                 *************/
                if (mouse_decode(&mdec, i - 512) != 0) {
                    mx += mdec.x;
                    my += mdec.y;
                    if (mx < 0) {
                        mx = 0;
                    }
                    if (my < 0) {
                        my = 0;
                    }
                    if (mx > binfo->scrnx - 1) {
                        mx = binfo->scrnx - 1;
                    }
                    if (my > binfo->scrny - 1) {
                        my = binfo->scrny - 1;
                    }
                    sheet_slide(sht_mouse, mx, my);
                    if ((mdec.btn & 0x01) != 0) { // left click
                        if (mmx < 0) { // normal mode
                            // lift up a clicked window to top
                            for (j = shtctl->top - 1; j > 0; j--) {
                                sht = shtctl->sheets[j];
                                x = mx - sht->vx0;
                                y = my - sht->vy0;
                                if (0 <= x && x < sht->bxsize && 0 <= y && y < sht->bysize) {
                                    if (sht->buf[y*sht->bxsize + x] != sht->col_inv) {
                                        sheet_updown(sht, shtctl->top-1);
                                        if (sht != key_win) {
                                            cursor_c = keywin_off(key_win, sht_win, cursor_c, cursor_x);
                                            key_win = sht;
                                            cursor_c = keywin_on(key_win, sht_win, cursor_c);
                                        }
                                        if (3 <= x && x < sht->bxsize-3 && 3 <= y && y < 21) {
                                            // if title is clicked, switch to move mode
                                            mmx = mx;
                                            mmy = my;
                                        }
                                        if (sht->bxsize-21 <= x && x < sht->bxsize-5 && 5 <= y && y < 19) { 
                                            // [x] clicked
                                            if ((sht->flags) & 0x10 != 0) { // closable window
                                                task = sht->task;
                                                cons_putstr0(task->cons, "\nBreak(mouse) :\n");
                                                io_cli();
                                                task->tss.eax = (int) &(task->tss.esp0);
                                                task->tss.eip = (int) asm_end_app;
                                                io_sti();
                                            }
                                        }
                                        break;
                                    }
                                }
                            }
                        } else { // move mode
                            x = mx - mmx;
                            y = my - mmy;
                            sheet_slide(sht, sht->vx0 + x, sht->vy0 + y);
                            mmx = mx;
                            mmy = my;
                        }
                    } else { // unclick
                        mmx = -1;
                    }
                }
            } else if (i <= 1) { // for the cursor
                /**************
                 *   cursor   *
                 **************/
                if (i != 0) {
                    timer_init(timer, &fifo, 0);
                    if (cursor_c >= 0) {
                        cursor_c = COL8_000000;
                    }
                } else {
                    timer_init(timer, &fifo, 1);
                    if (cursor_c >= 0) {
                        cursor_c = COL8_FFFFFF;
                    }
                }
                timer_settime(timer, 50);
                if (cursor_c >= 0) {
                    boxfill8(sht_win->buf, sht_win->bxsize, cursor_c, cursor_x, 28, cursor_x + 7, 43);
                    sheet_refresh(sht_win, cursor_x, 28, cursor_x + 8, 44);
                }
            }
        }
    }
}


int keywin_off(struct SHEET *key_win, struct SHEET *sht_win, int cur_c, int cur_x) {
    change_wtitle8(key_win, 0);
    if (key_win == sht_win) {
        cur_c = -1;
        boxfill8(sht_win->buf, sht_win->bxsize, COL8_FFFFFF, cur_x, 28, cur_x + 7, 43);
    } else {
        if ((key_win->flags & 0x20) != 0) {
            fifo32_put(&key_win->task->fifo, 3);
        }
    }
    return cur_c;
}

int keywin_on(struct SHEET *key_win, struct SHEET *sht_win, int cur_c) {
    change_wtitle8(key_win, 1);
    if (key_win == sht_win) {
        cur_c = COL8_000000;
    } else {
        if ((key_win->flags & 0x20) != 0) {
            fifo32_put(&key_win->task->fifo, 2);
        }
    }
    return cur_c;
}
