#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

typedef int compare_function(void const*, void const*);

/* wrapper function for generic sort 
** base:   array of generic type
** n:      number of objects to be sorted
** size:   size of type of base array
** compar: pointer to function that operates on 
**         given type
**         returns 1 if a > b
**                 0 if a = b
**                -1 if a < b  *    void* leftArr = malloc(size * nL);
*/
void genericSort(void* base, size_t n, 
                 size_t size, compare_function* compar);
/* generic merge helper function */
void merge(void* base, size_t l, size_t m, size_t r,
           size_t size, compare_function* compar);
/* generic merge sort function */
void mergeSort(void* base, size_t l, size_t r,
               size_t size, compare_function* compar);
/* unsigned int compare function */
int compare_unsigned(void const* a, void const* b);

int main(void) {
    int arr[] = { 2, 4, 3, 6 };
    printf("%ld\n", sizeof(int));
    //genericSort(arr, 4, sizeof(int), compare_unsigned);
    merge(arr, 0, 16, 32, 4, compare_unsigned);
    return 1;
}

void genericSort(void* base, size_t n, size_t size, compare_function* compar) {
    printf("genericSort\n");
    mergeSort(base, 0, n * size - size, size, compar);
}

void mergeSort(void* base, size_t l, size_t r, size_t size, compare_function* compar) {
    printf("mergeSort\n");
    //base case
    if (l < r) {
        printf("l = %zu, r = %zu\n", l, r);
        // calculate typed index of generic array
        size_t lIndex = 0;
        if (l) lIndex = l / size;
        size_t rIndex = r / size; 
        // determine middle index of generic array
        size_t m = (lIndex + (rIndex - lIndex) / 2) * size;

        // sort left and right halves of sub array
        mergeSort(base, l, m, size, compar);
        mergeSort(base, m + size, r, size, compar);
        // merge sorted halves
        printf("merge\n");
        merge(base, l, m, r, size, compar);
    }
}

void merge(void* base, size_t l, size_t m, size_t r, size_t size, compare_function* compar) {
    printf("merge/n");
    // calculate size of temp arrays
    size_t nL = m - l;
    size_t nR = r - m + size;

    // sub array index iterators 
    size_t mergeIndex = l, leftIndex = 0, rightIndex = 0;

    // initialize and populate temp arrays
    uint8_t leftArr[size * nL];
    uint8_t rightArr[size * nR];
    uint8_t* baseArr = *((uint8_t*) base);
    /*
    void* leftArr = malloc(size * nL);
    void* rightArr = malloc(size * nR);
    */
    for (size_t i = 0; i < nL; ++i) {
        *(leftArr + i) = *(baseArr + l + i);
    }
    for (size_t i = 0; i < nR; ++i) {
        *(rightArr + i) = *(baseArr + m + i);
    }

    // merge left and right arrays in order 
    while (leftIndex < nL && rightIndex < nR) {
        if (compar(&leftArr[leftIndex], &rightArr[rightIndex]) < 1) {
            for (size_t i = 0; i < size; ++i) {
                *(baseArr + mergeIndex) = *(leftArr + leftIndex);
                ++mergeIndex;
                ++leftIndex;
            }
        }
        else {
            for (size_t i = 0; i < size; ++i) {
                *(baseArr + mergeIndex) = *(rightArr + rightIndex);
                ++mergeIndex;
                ++rightIndex;
            }
        }    
    }
}

int compare_unsigned(void const* a, void const* b) {
    unsigned const* A = a;
    unsigned const* B = b;
    if (*A < *B) return -1;
    else if (*A > *B) return +1;
    else return 0;
}


