/*
    2020.07.08 ysChoi
    Merge Sort 알고리즘 구현
    분할(Divide) & 정복(Conquer) 알고리즘
    최선의 경우: O(NlogN)
    평균이 경우: O(NlogN)
    최악의 경우: O(NlogN)
        -> 항상 둘로 분리하기 때문에 O(NlogN) 성능 보장
    임시 Array를 사용해야 하기 때문에 추가적인 메모리가 필요하다는 단점 존재
*/
#include <iostream>

void merge(int *arr, int left, int mid, int right) {
    int l = left, r = mid + 1, k = 0;

    int *tempArr = (int*)malloc(sizeof(int) * (right - left + 1));

    while((l <= mid) && (r <= right)) {
        if(arr[l] <= arr[r]) {
            tempArr[k++] = arr[l++];
        } else {
            tempArr[k++] = arr[r++];
        }
    }

    while(l <= mid) {
        tempArr[k++] = arr[l++];
    }

    while(r <= right) {
        tempArr[k++] = arr[r++];
    }

    for(int i = 0; i < k; i++) {
        arr[left++] = tempArr[i];
    }

    free(tempArr);
    tempArr = NULL;
}

void merge_sort(int *arr, int left, int right) {
    if(left < right) {
        int mid = (left + right) / 2;

        merge_sort(arr, left, mid);
        merge_sort(arr, mid + 1, right);

        merge(arr, left, mid, right);
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
    merge_sort(arr, 0, 10 - 1);

    for(int i = 0; i < 10; i++) {
        if(i != 0) std::cerr << ", ";
        std::cerr << arr[i];
    }
    std::cerr << std::endl;

    return 0;
}
