void api_end(void);

void HariMain(void) {
    *((char *) 0x00102600) = 0; // set a head of root directory to 0 (end of the list)
    api_end();
}
