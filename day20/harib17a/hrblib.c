//#include <stdio.h>

int _hrb_sprint_dec(char **dest, int num) {
    int res = 1;
    if (num > 9) {
        res += _hrb_sprint_dec(dest, num/10);
    }
    **dest = num%10 + '0';
    (*dest)++;
    return res;
}

int _hrb_sprint_hex(char **dest, unsigned int num) {
    int lx, res = 1;

    if (num > 15) {
        res += _hrb_sprint_hex(dest, num >> 4);
    }

    lx = num&15;
    if (lx < 10) {
        **dest = lx + '0';
    } else {
        **dest = (lx-10) + 'a';
    }
    (*dest)++;
    return res;
}

int _hrb_sprint_HEX(char **dest, unsigned int num) {
    int lx, res = 1;

    if (num > 15) {
        res += _hrb_sprint_HEX(dest, num >> 4);
    }

    lx = num&15;
    if (lx < 10) {
        **dest = lx + '0';
    } else {
        **dest = (lx-10) + 'A';
    }
    (*dest)++;
    return res;
}

int _hrb_sprint_int(char **dest, char fmt, int num) {
    int res = 0;
    if (num == 0) {
        **dest = '0';
        (*dest)++;
        res++;
    } else if (fmt == 'd') {
        if (num < 0) {
            **dest = '-';
            (*dest)++;
            num = -num;
            res++;
        }
        res += _hrb_sprint_dec(dest, num);
    } else if (fmt == 'x') {
        res = _hrb_sprint_hex(dest, (unsigned int) num);
    } else if (fmt == 'X') {
        res = _hrb_sprint_HEX(dest, (unsigned int) num);
    }
    **dest = 0x00;
    return res;
}

int hrb_str2num(char **fmt) {
    int num = 0;
    for (; '0' <= **fmt && **fmt <= '9'; (*fmt)++) {
        //printf("%c\n", **fmt);
        num = num*10 + (**fmt)-'0';
    }
    return num;
}

void hrb_sprintfi(char *dest, char *fmt, int *nums) {
    char buf[10], *p, zero;
    int pad, res, i; 
    for (; *fmt != 0x00; fmt++) {
        //printf("%c\n", *fmt);
        if (*fmt == '%') {
            fmt++; // skip %
            zero = 0;
            if (*fmt == '0') {
                fmt++;
                zero = 1;
            }
            pad = hrb_str2num(&fmt);
            p = buf;
            res = _hrb_sprint_int(&p, *fmt, *nums);
            //printf("%d %d %s\n", pad, res, buf);
            for (i = 0; i < pad-res; i++, dest++) {
                *dest = zero ? '0' : ' ';
            }
            for (i = 0; i < res; i++, dest++) {
                *dest = buf[i];
                //printf("%p %p\n", dest, buf+i);
            }
            nums++;
        } else {
            *dest = *fmt;
            dest++;
        }
    }
    *dest = 0x00;
    return;
}

int hrb_strcmp(char *s, char *t) {
    for (; *s != 0 || *t != 0; s++, t++) {
        if (*s < *t) return -1;
        if (*s > *t) return 1;
    }
    return 0;
}

int hrb_strcmp_len(char *s, char *t, int len) {
    int i;
    for (i = 0; i < len && (s[i] != 0 || t[i] != 0); i++) {
        if (s[i] < t[i]) return -1;
        if (s[i] > t[i]) return 1;
    }
    return 0;
}

char* hrb_strcpy(char *s, char *t) {
    for(; *t != 0; s++, t++) {
        *s = *t;
    }
    *s = 0;
    return s;
}

/*
int main(void) {
    int args[3];
    char s[256];
    args[0] = 10;
    args[1] = 12;
    args[2] = 33;
    hrb_sprintfi(s, "hello %d hoge %04x piyo %05d", args);
    puts(s);
    return 0;
}
*/
