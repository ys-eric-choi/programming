/*
    2020.07.08 ysChoi
    Heap Sort 알고리즘 구현
    MAX Heap Tree 구조를 이용한 정렬 방식
    MAX Heap Tree는
        1. 완전 이진 트리여야 한다
            - 자식노드가 2개 이하
            - 각 depth에서 왼쪽부터 값이 채워져야 한다
                -> 이 성질을 이용하여 Heap Tree를 배열로 나타내면
                       1) 배열에서 부모 노드의 위치는 자식 노드의 N / 2에 위치
                       2) 배열에서 부모 노드의 왼쪽 자식 노드의 위치는 2N에 위치
                       3) 배열에서 부모 노드의 오른쪽 자식 노드의 위치는 2N + 1에 위치
        2. 트리 내 모든 부모-자식 노드 관계에서
            - 부모 노드의 값이 자식 노드의 값보다 커야 한다
                -> 반대가 되면 MIN Heap Tree
    heapify에서 비교 횟수는 트리의 높이 = O(logN)
    N - 1번 수행 = O(N)
    따라서 전체 수행 시간은 다음과 같음
    최선의 경우: O(NlogN)
    평균의 경우: O(NlogN)
    최악의 경우: O(NlogN)
*/
#include <iostream>

// 부모 노드와 자식 노드 중 가장 큰 값을 부모 노드 값으로 교체
void heapify(int *arr, int current, int size) {
    int left_child = current * 2 + 1;
    int right_child = current * 2 + 2;
    int new_current = current;
    int temp;

    // size(배열 길이)보다 큰지 체크
    if((left_child < size) && (arr[new_current] < arr[left_child])) {
        new_current = left_child;
    }
    // size(배열 길이)보다 큰지 체크
    if((right_child < size) && (arr[new_current] < arr[right_child])) {
        new_current = right_child;
    }

    // 더 큰 값이 있으면 값을 가장 큰 값으로 swap 후 해당 위치부터 다시 재귀적 검사
    if(new_current != current) {
        temp = arr[new_current];
        arr[new_current] = arr[current];
        arr[current] = temp;
        heapify(arr, new_current, size);
    }
}

// MAX Heap Tree 만들기
void build_max_heap(int *arr, int size) {
    // 전체 배열의 중간 지점부터 시작 = 마지막 노드의 부모 노드부터 시작한다는 의미
    int current = size / 2 - 1;

    for( ; current > -1; current--) {
        heapify(arr, current, size);
    }

    for(int i = 0; i < 10; i++) {
        if(i != 0) std::cerr << ", ";
        std::cerr << arr[i];
    }
    std::cerr << std::endl;
}

void heap_sort(int *arr, int size) {
    // Step1: MAX Heap Tree 생성
    build_max_heap(arr, size);

    int temp;
    int current;

    // Step2: MAX Heap Tree에서 root의 값과 Array의 마지막 값을 swap하고
    //        root 위치부터 다시 heapify 수행
    while(size > 0) {
        temp = arr[0];
        arr[0] = arr[size - 1];
        arr[size - 1] = temp;

        size--;
        heapify(arr, 0, size);
    }
}

int main(void) {

    int arr[10] = {1, 9, 7, 3, 2, 5, 8, 4, 0, 6};

    for(int i = 0; i < 10; i++) {
        if(i != 0) std::cerr << ", ";
        std::cerr << arr[i];
    }
    std::cerr << std::endl;

    heap_sort(arr, 10);

    for(int i = 0; i < 10; i++) {
        if(i != 0) std::cerr << ", ";
        std::cerr << arr[i];
    }
    std::cerr << std::endl;

    return 0;
}
