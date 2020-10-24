int api_openwin(char *buf, int xsz, int ysz, int col_inv, char *title);
void api_boxfilwin(int win, int x0, int y0, int x1, int y1, int col);
void api_initmalloc(void);
char *api_malloc(int size);
void api_point(int win, int x, int y, int col);
void api_refreshwin(int win, int x0, int y0, int x1, int y1);
void api_end(void);

int hrb_rand(void);

void HariMain(void) {
    char *buf;
    int win, i, x, y;

    api_initmalloc();
    buf = api_malloc(150*50);
    win = api_openwin(buf, 150, 100, -1, "stars");
    api_boxfilwin(win, 6, 26, 143, 93, 0);
    for (i = 0; i < 50; i++) {
        x = (hrb_rand()%137) + 6;
        y = (hrb_rand()%67) + 26;
        api_point(win + 1, x, y, 3);
    }
    api_refreshwin(win, 6, 26, 144, 94);
    api_end();
}
