// 고정 char배열에 문자열 복사 시 배열의 size와 문자열 길이 중 더 작은 것 만큼만 복사하기 위한 함수
#define STRCPY(DEST, SRC) { \
    int i, iDestSize = sizeof(DEST) - 1; \
    for(i = 0; (i < iDestSize) && (SRC[i] != '\0') ; i++) { \
        DEST[i] = SRC[i]; \
    } \
    DEST[i] = '\0'; \
}
