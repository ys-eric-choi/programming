// 2020.05.19
// XOR 연산을 이용한 암호화/복호화 예시
#include <iostream>

#define FREE(X) { free(X); X = NULL; }

int main(void) {

    FILE *fp = NULL;

    fp = fopen("test.bin", "wb");

    std::string strInput = "안녕하세요. ys-eric-choi 계정 입니다.";

    // 암호화 Key
    const char* strKey = "@ys-eric-choi";
    int iKeyLen = strlen(strKey);

    int iLen = strInput.length();

    // 실제 출력 문장 길이를 먼저 출력
    fwrite(&iLen, sizeof(int), 1, fp);

    char* strOut = (char*)strInput.c_str();
    char c;

    for(int i = 0; i < iLen; i++) {
        // XOR 연산으로 암호화
        c = strOut[i] ^ strKey[i % iKeyLen];
        fwrite(&c, sizeof(char), 1, fp);
    }

    fflush(fp);
    fclose(fp);

    fp = fopen("test.bin", "rb");

    // 읽어야 하는 문장의 길이를 먼저 구함
    fread(&iLen, sizeof(int), 1, fp);

    // NULL문자('\0') 삽입을 위해 길이보다 +1 만큼 메모리를 할당한다.
    char* buf = (char*) malloc(sizeof(char) * (iLen + 1));

    for(int i = 0; i < iLen; i++) {
        fread(&c, sizeof(char), 1, fp);
        // XOR 연산으로 복호화
        buf[i] = c ^ strKey[i % iKeyLen];
    }

    buf[iLen] = '\0';
    std::cerr << ">>>" << buf << "<<<" << std::endl;

    fclose(fp);

    // 동적할당 메모리 해제
    FREE(buf);

    return 0;
}
