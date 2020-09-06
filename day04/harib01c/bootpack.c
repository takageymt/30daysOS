void io_hlt(void);
void write_mem8(int addr, int data);

void HariMain(void) {
    char *p;
    int i;
    for (i = 0xa0000; i < 0xb0000; i++) {
        //write_mem8(i, i & 0x0f);
        p = (char *) i;
        *p = i & 0x0f;
    }
    for (;;) {
        io_hlt();
    }
}
