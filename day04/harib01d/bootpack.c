void io_hlt(void);
void write_mem8(int addr, int data);

void HariMain(void) {
    char *p;
    int i;

    p = (char *) 0x0000;
    for (i = 0xa0000; i < 0xb0000; i++) {
        *(p + i) = i & 0x0f;
    }
    for (;;) {
        io_hlt();
    }
}
