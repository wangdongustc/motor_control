#include "quick_sort.h"
Range new_Range(int s, int e) {
    Range r;
    r.start = s;
    r.end = e;
    return r;
}
inline void swap(int *x, int *y) {
    int t = *x;
    *x = *y;
    *y = t;
}
void quick_sort(int arr[], int index[], const int len) {
    if (len <= 0)
        return; 
    Range* r = new Range[len];
    int p = 0;
    r[p++] = new_Range(0, len - 1);
    while (p) {
        Range range = r[--p];
        if (range.start >= range.end)
            continue;
        int mid = arr[range.end];
        int left = range.start, right = range.end - 1;
        while (left < right) {
            while (arr[left] < mid && left < right)
                left++;
            while (arr[right] >= mid && left < right)
                right--;
            swap(&arr[left], &arr[right]);
            swap(&index[left], &index[right]);
        }
        if (arr[left] >= arr[range.end]) {
            swap(&arr[left], &arr[range.end]);
            swap(&index[left], &index[range.end]);
        }
        else
            left++;
        r[p++] = new_Range(range.start, left - 1);
        r[p++] = new_Range(left + 1, range.end);
    }
    delete[] r;
}