
int hrb_rand(void) {
    static int X = 1;
    static int A = 1664525;
    static int C = 1013904223;
    static int M = 2147483647;
    X = (X * A + C) & M;
    return X;
}
