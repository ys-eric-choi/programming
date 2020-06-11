#include <iostream>
#include <cstring>
#include "util.h"

int main(void) {

    char a[10] = "123456789";
    char b[5] = "aaaa";

    STRCPY(a, b);
    fprintf(stderr, "%s\n", a);

    for(int i = 0; i < 10; i++) {
        fprintf(stderr, "'%c'", a[i]);
    }
    fprintf(stderr, "\n");

    return 0;
}
