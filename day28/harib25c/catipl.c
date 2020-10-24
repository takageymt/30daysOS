#include "apilib.h"

void HariMain(void) {
    int fh;
    char c;
    fh = api_fopen("ipl10.asm");
    if (fh != 0) {
        for (;;) {
            if (api_fread(&c, 1, fh) == 0) {
                break;
            }
            api_putchar(c);
        }
    } else {
        api_putstr0("FILE NOT FOUND...");
    }
    api_end();
}
