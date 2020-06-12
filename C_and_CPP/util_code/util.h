// 고정 char배열에 문자열 복사 시 배열의 size와 문자열 길이 중 더 작은 것 만큼만 복사하기 위한 함수
#define STRCPY(DEST, SRC) { \
    int i, iDestSize = sizeof(DEST) - 1; \
    for(i = 0; (i < iDestSize) && (SRC[i] != '\0') ; i++) { \
        DEST[i] = SRC[i]; \
    } \
    DEST[i] = '\0'; \
}

// 문자열 치환 함수, 첫 번째로 발견된 문자열만 치환
// 반드시 함수 외부에서 DEST를 free 시킬 것!
#define REPLACE(DEST, SRC, FIND, VALUE) { \
    size_t iSrcLen = strlen(SRC); \
    size_t iValueLen = strlen(VALUE); \
    DEST = (char*)calloc((iSrcLen + iValueLen + 1), sizeof(char)); \
    char *ptrFind = NULL; \
    ptrFind = strstr(SRC, FIND) ; \
    if(ptrFind == NULL) { \
        strcpy(DEST, SRC); \
    } else { \
        size_t iFindLen = strlen(ptrFind); \
        strncpy(DEST, SRC, iSrcLen - iFindLen); \
        strcat(DEST, VALUE); \
        strcat(DEST, ptrFind + strlen(FIND)); \
    } \
}
