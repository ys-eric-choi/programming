/*
	2020.02.18
	struct를 선언하면서 초기화 하는 방법에 대한 예제
	class의 생성자와 유사
*/

#include <iostream>

using namespace std;

typedef struct Person {
	int age;
	Person() : age(33) {}
} Person_;

struct Car {
	int number;
	Car() : number(1234) {}
};

int main(void) {

	Person_ ys;
	cerr << ys.age << endl;

	Car car;
	cerr << car.number << endl;

	return 0;
}
