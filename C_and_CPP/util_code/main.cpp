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

    char org[] = "I am a boy!";
    char find[] = "I am";
    char value[] = "You are";

    char *result = NULL;

    fprintf(stderr, "%s\n", org);

    REPLACE(result, org, find, value);

    fprintf(stderr, "%s\n", result);

    char find2[] = "boy";
    char value2[] = "girl";

    char *result2 = NULL;

    REPLACE(result2, result, find2, value2);

    fprintf(stderr, "%s\n", result2);

    free(result);
    free(result2);

    return 0;
}
