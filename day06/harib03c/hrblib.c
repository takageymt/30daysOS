//#include <stdio.h>

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

void _hrb_sprint_int(char **dest, char fmt, int num) {
    if (num == 0) {
        **dest = '0';
        //(*dest)++;
        *(*dest+1) = 0x00;
        return;
    }
    if (fmt == 'd') {
        if (num < 0) {
            **dest = '-';
            (*dest)++;
            num = -num;
        }
        _hrb_sprint_dec(dest, num);
    } else if (fmt == 'x') {
        _hrb_sprint_hex(dest, (unsigned int) num);
    }
    **dest = 0x00;
    return;
}

void hrb_sprintfi(char *dest, char *fmt, int *nums) {
    for (; *fmt != 0x00; dest++, fmt++) {
        if (*fmt == '%') {
            fmt++; // skip %
            _hrb_sprint_int(&dest, *fmt, *nums);
            nums++;
            dest--; // delete null
        } else {
            *dest = *fmt;
        }
    }
    *dest = 0x00;
    return;
}

/*
int main(void) {
    int args[3];
    char s[256];
    args[0] = 10;
    args[1] = 10;
    args[2] = 33;
    hrb_sprintfi(s, "hello %d hoge %x piyo %d", args);
    puts(s);
    return 0;
}
*/
