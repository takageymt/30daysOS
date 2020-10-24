#include "hrblib.h"
#include "apilib.h"

#define MAX 10000

void HariMain(void) {
    char flag[MAX], s[8];
    int arg[2];
    int i, j;
    api_show_esp();
    for (i = 0; i < MAX; i++) {
        flag[i] = 0;
    }
    for (i = 2; i < MAX; i++) {
        if (flag[i] == 0) {
            arg[0] = i;
            hrb_sprintfi(s, "%d ", arg);
            //api_putstr0(s);
            for (j = i*2; j < MAX; j += i) {
                flag[j] = 1;
            }
        }
    }
    api_show_esp();
    api_end();
}
