/*
    2020.07.09
    Linked List를 이용한 Stack 자료구조 구현
*/

#include <iostream>

// Linked List를 위한 구조체
typedef struct Node {
    int value;
    Node* nextNode;
    
    Node() : value(0), nextNode(0) {}
} Node_;

// Stack 최상단 Node를 가리키는 포인터
Node_ *topNode;

// Stack이 비어있는지 체크
bool isEmpty() {
    if(topNode == NULL) return true;
    else return false;
}

// Stack에 새로운 데이터 추가
void push(int value) {
    // 1. 신규 데이터를 저장할 메모리 할당
    Node_ *newNode = new Node_;
    // 2. 신규 데이터 저장
    newNode->value = value;
    // 3. 현재 최상단 Node를 다음 Node로 저장
    newNode->nextNode = topNode;
    // 4. 최상단 Node를 가리키는 포인터를 현재 Node로 변경
    topNode = newNode;
}

// Stack에 최상단 Node 삭제
void pop() {
    // 1. 최상단 Node를 변경하기 위해 최상단 Node의
    //    아래 Node를 가리키는 포인터를 임시 변수에 저장
    Node_ *tempNode = topNode->nextNode;
    // 2. 최상단 Node 삭제
    delete topNode;
    // 3. 최상단 Node를 변경
    topNode = tempNode;
}

// Stack 전체 데이터 출력 함수
void printStack() {
    if(isEmpty()) {
        std::cerr << "Stack is Empty!" << std::endl;
    } else {
        Node_ *currentNode = topNode;
        do {
            std::cerr << currentNode->value << std::endl;
            currentNode = currentNode->nextNode;
        } while(currentNode != NULL);
    }
}

// Stack 전체 데이터 삭제 함수
void clearStack() {
    // Stack이 비어이지 않을 때까지 pop() 반복
    while(!isEmpty()) {
        pop();    
    }
}

int main(void) {

    topNode = NULL;

    push(0);
    push(1);
    push(2);

    printStack();

    pop();
    printStack();

    clearStack();
    printStack();

    return 0;
}

