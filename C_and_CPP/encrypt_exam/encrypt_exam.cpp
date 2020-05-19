#include <iostream>

#define FREE(X) { free(X); X = NULL; }

int main(void) {

    FILE *fp = NULL;

    fp = fopen("test.bin", "wb");

    std::string strInput = "안녕하세요. ys-eric-choi 계정 입니다.";

    const char* strKey = "@ys-eric-choi";
    int iKeyLen = strlen(strKey);

    int iLen = strInput.length();

    fwrite(&iLen, sizeof(int), 1, fp);

    char* strOut = (char*)strInput.c_str();
    char c;

    for(int i = 0; i < iLen; i++) {
        c = strOut[i] ^ strKey[i%iKeyLen];
        fwrite(&c, sizeof(char), 1, fp);
    }

    fflush(fp);
    fclose(fp);

    fp = fopen("test.bin", "rb");

    fread(&iLen, sizeof(int), 1, fp);

    char* buf = (char*) malloc(sizeof(char) * iLen + 1);

    for(int i = 0; i < iLen; i++) {
        fread(&c, sizeof(char), 1, fp);
        buf[i] = c ^ strKey[i%iKeyLen];
    }

    buf[iLen] = '\0';
    std::cerr << ">>>" << buf << "<<<" << std::endl;

    fclose(fp);

    FREE(buf);

    return 0;
}
