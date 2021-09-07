/* Implementation of mergeSort that operates on generic data types
** challenge problem 14 from Modern C
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>

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
/* generic mergeSort function */
void mergeSort(void* base, size_t l, size_t r,
               size_t size, compare_function* compar);
/* compare functions */
int compare_unsigned(void const* a, void const* b);
int compare_int(void const* a, void const* b);
int compare_string(void const* a, void const* b);

int main(void) {
    char* stringArr[] = { "cat", "a", "coco" };
    printf("size of char* = %ld\n", sizeof(char*));
    genericSort(stringArr, 3, sizeof(char*), compare_string);
    for (size_t i = 0; i < 3; ++i) {
        printf("%s\n", stringArr[i]);
    }
    return 1;
}

void genericSort(void* base, size_t n, size_t size, compare_function* compar) {
    mergeSort(base, 0, n * size - 1, size, compar);
}

void mergeSort(void* base, size_t l, size_t r, size_t size, compare_function* compar) {
    //base case
    if (r - l > size - 1) {
        // determine middle index of generic array
        size_t typeIndexL = l / size;
        size_t typeIndexR = (r + 1) / size;
        size_t m = (typeIndexL + (typeIndexR - typeIndexL) / 2) * size;

        // sort left and right halves of sub array
        if (r - l > size * 2 - 1) {
            mergeSort(base, l, m - 1, size, compar);
            mergeSort(base, m, r, size, compar);
        }

        // merge sorted halves
        merge(base, l, m, r, size, compar);
    }
}

void merge(void* base, size_t l, size_t m, size_t r, size_t size, compare_function* compar) {
    // calculate size of temp arrays
    size_t nL = m - l;
    size_t nR = r - m + 1;

    // sub array index iterators 
    size_t mergeIndex = l, leftIndex = 0, rightIndex = 0;

    // initialize and populate temp arrays
    uint8_t leftArr[nL];
    uint8_t rightArr[nR];
    // cast void* base as array of unint8_t for bytewise manipulation 
    uint8_t* baseArr = ((uint8_t*) base);

    for (size_t i = 0; i < nL; ++i) {
        leftArr[i] = baseArr[l + i];
    }
    for (size_t i = 0; i < nR; ++i) {
        rightArr[i] = baseArr[m + i];
    }
    
    // merge left and right arrays into base array in order
    while (leftIndex < nL && rightIndex < nR) {
        // pass pointers to current index of both arrays to 
        // given compare function
        if (compar(&leftArr[leftIndex], &rightArr[rightIndex]) < 1) {
            // copy size bytes from lesser value to base array
            for (size_t i = 0; i < size; ++i) {
                baseArr[mergeIndex] = leftArr[leftIndex];
                ++mergeIndex;
                ++leftIndex;
            }
        }
        else {
            for (size_t i = 0; i < size; ++i) {
                baseArr[mergeIndex] = rightArr[rightIndex];
                ++mergeIndex;
                ++rightIndex;
            }
        }    
    }

    // insert remaining values in to base array
    while (leftIndex < nL) {
        baseArr[mergeIndex] = leftArr[leftIndex];
        ++mergeIndex;
        ++leftIndex;
    }

    while (rightIndex < nR) {
        baseArr[mergeIndex] = rightArr[rightIndex];
        ++mergeIndex;
        ++rightIndex;
    }

}

int compare_unsigned(void const* a, void const* b) {
    unsigned const* A = a;
    unsigned const* B = b;
    if (*A < *B) return -1;
    else if (*A > *B) return +1;
    else return 0;
}

int compare_int(void const* a, void const* b) {
    int const* A = a;
    int const* B = b;
    if (*A < *B) return -1;
    else if (*A > *B) return +1;
    else return 0;
}

int compare_string(void const* a, void const* b) {
    // casts a and b to pointers to arrays of char*
    char* const* ptrA = a;
    char* const* ptrB = b;

    return strcmp(*ptrA, *ptrB);
}

