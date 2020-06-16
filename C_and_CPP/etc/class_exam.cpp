/*
    2020.06.17
    C++ class의 생성자 소멸자 호출 테스트
    함수 내에서 생성한 인스턴스의 생성자와 소멸자 호출 시점 확인
    소멸자가 정상적으로 호출되고 동적으로 할당한 메모리까지 정상적으로 해제되는 것까지 확인
        $ g++ -g class_exam.cpp
        $ valgrind --leak-check=full ./a.out
*/

#include <iostream>
#include <cstring>

class Test {
public:
    Test() { // 생성자
        std::cerr << "생성됨!" << std::endl;
        // 생성자 내에서 멤버변수 초기화
        this->strValue = NULL;
    };

    Test(const char* strSrc) { // 인자가 있는 생성자
        std::cerr << "생성됨!" << std::endl;
        // 생성자 내에서 멤버변수를 파라미터 값으로 할당
        this->strValue = strdup(strSrc);
    };

    ~Test() { // 소멸자
        std::cerr << "소멸됨!" << std::endl;
        // 소멸자 내에서 동적으로 할당한 메모리 해제
        free(this->strValue); // 포인터가 NULL이면 아무런 동작을 하지 않으니 NULL 체크하지 말자
        this->strValue = NULL; // 단, 해제 이후 항상 NULL로 초기화 시키는 습관을 들이자 (매크로 만들면 편함)
    };

    char* GetValue() {
        return this->strValue;
    }

    void SetValue(const char* strSrc) {
        free(this->strValue); // NULL로 초기화되지 않은 pointer를 free하면 에러 발생에 주의
        this->strValue = strdup(strSrc);
    }

private:
    char *strValue;
};

// 생성자/소멸자 테스트 함수
void func_test() {
    Test test; // 지역변수 인스턴스 생성과 함께 생성자 호출

    // 멤버 변수에 값 할당
    test.SetValue("생성자 소멸자 테스트!");
    for(int i = 0; i < 10; i++) {
       std::cerr << i << std::endl;
    }
    std::cerr << test.GetValue() << std::endl;
    // 함수가 종료 되면서 지역변수 인스턴스가 메모리 해제되며 소멸자 호출됨
}

int main(void) {
    std::cerr << "함수 호출" << std::endl;
    func_test();
    std::cerr << "함수 종료" << std::endl;

    // 동적 인스턴스 생성을 통한 명시적인 생성자, 소멸자 호출
    Test *test = new Test("동적 할당 테스트");
    std::cerr << test->GetValue() << std::endl;
    delete test;

    return 0;
}
