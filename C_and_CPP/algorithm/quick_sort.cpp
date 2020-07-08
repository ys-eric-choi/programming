/*
    2020.07.08 ysChoi
    Quick Sort 알고리즘 구현
    분할(Divide) & 정복(Conquer) 알고리즘
    최선의 경우: O(NlogN)
    평균이 경우: O(NlogN)
    최악의 경우: O(N^2)
        -> Array가 이미 정렬되어 있고
           Pivot을 처음 또는 마지막 값으로 사용하는 경우
*/
#include <iostream>

int partition(int *arr, int left, int right) {
    // pivot을 중간값으로 지정
    int pivot = arr[(left + right) / 2];
    int temp;

    while(left < right) { // left와 right가 교차되기 전까지만 수행
        // left 위치의 값이 pivot 보다 작으면 left 위치를 오른쪽으로 이동
        while(arr[left] < pivot) left++;
        // right 위치의 값이 pivot 보다 크면 right 위치를 왼쪽으로 이동
        while(arr[right] > pivot) right--;

        // Array내 중복 값이 있는 경우를 위한 예외 처리
        if(arr[left] == arr[right]) {
            left++;
        } else if(left < right) { // left와 right값이 교차되기 전이라면 swap 수행
            temp = arr[left];
            arr[left] = arr[right];
            arr[right] = temp;
        }
    }

    return right; // left or right
}

void quick_sort(int *arr, int left, int right) {

    if(left < right) {
        int pivot_idx = partition(arr, left, right);

        quick_sort(arr, left, pivot_idx - 1);
        quick_sort(arr, pivot_idx + 1, right);
    }
}

int main(void) {

    int arr[10] = {1, 9, 7, 3, 2, 5, 8, 4, 0, 6};

    for(int i = 0; i < 10; i++) {
        if(i != 0) std::cerr << ", ";
        std::cerr << arr[i];
    }
    std::cerr << std::endl;

    // 초기 right 값을 Array의 길이 - 1 값으로 입력에 주의
    quick_sort(arr, 0, 10 - 1);

    for(int i = 0; i < 10; i++) {
        if(i != 0) std::cerr << ", ";
        std::cerr << arr[i];
    }
    std::cerr << std::endl;

    return 0;
}
