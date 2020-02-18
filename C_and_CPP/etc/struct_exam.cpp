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
