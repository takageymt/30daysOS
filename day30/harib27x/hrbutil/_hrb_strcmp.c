
int hrb_strcmp(char *s, char *t) {
    for (; *s != 0 || *t != 0; s++, t++) {
        if (*s < *t) return -1;
        if (*s > *t) return 1;
    }
    return 0;
}

int hrb_strncmp(char *s, char *t, int len) {
    int i;
    for (i = 0; i < len && (s[i] != 0 || t[i] != 0); i++) {
        if (s[i] < t[i]) return -1;
        if (s[i] > t[i]) return 1;
    }
    return 0;
}
