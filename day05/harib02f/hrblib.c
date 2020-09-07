#include <stdio.h>

void _hrb_sprint_dec(char **dest, int num) {
    if (num > 9) {
        _hrb_sprint_dec(dest, num/10);
    }
    **dest = num%10 + '0';
    (*dest)++;
    return;
}

void _hrb_sprint_hex(char **dest, unsigned int num) {
    int lx;

    if (num > 15) {
        _hrb_sprint_hex(dest, num >> 4);
    }

    lx = num&15;
    if (lx < 10) {
        **dest = lx + '0';
    } else {
        **dest = (lx-10) + 'a';
    }
    (*dest)++;
    return;
}

void hrb_sprint_int(char *dest, char fmt, int num) {
    char *tmp = dest;

    if (num == 0) {
        dest[0] = '0';
        dest[1] = 0x00;
        return;
    }
    if (fmt == 'd') {
        if (num < 0) {
            dest[0] = '-';
            dest++;
            num = -num;
        }
        _hrb_sprint_dec(&dest, num);
    } else if (fmt == 'x') {
        _hrb_sprint_hex(&dest, (unsigned int) num);
    }
    *dest = 0x00;
    dest = tmp;
    return;
}


int main(void) {
    char s[256];
    hrb_sprint_int(s, 'd', 12345);
    puts(s);
    hrb_sprint_int(s, 'd', -12345);
    puts(s);
    hrb_sprint_int(s, 'x', 456);
    puts(s);
    hrb_sprint_int(s, 'x', -456);
    puts(s);
    return 0;
}
