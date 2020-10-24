
char* hrb_strcpy(char *s, char *t) {
    for(; *t != 0; s++, t++) {
        *s = *t;
    }
    *s = 0;
    return s;
}

