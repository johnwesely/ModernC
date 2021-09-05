#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>

void printBits(size_t const size, void const * const ptr)
{
    unsigned char *b = (unsigned char*) ptr;
    unsigned char byte;
    int i, j;
    
    for (i = size-1; i >= 0; i--) {
        for (j = 7; j >= 0; j--) {
            byte = (b[i] >> j) & 1;
            printf("%u", byte);
        }
    }
    puts("");
}

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
    printf("genericSort\n");
    mergeSort(base, 0, n * size - 1, size, compar);
}

void mergeSort(void* base, size_t l, size_t r, size_t size, compare_function* compar) {
    printf("mergeSort\n");
    printf("l = %zu, r = %zu\n", l, r);
    //base case
    if (r - l > size - 1) {
        // determine middle index of generic array
        size_t typeIndexL = l / size;
        size_t typeIndexR = (r + 1) / size;
        size_t m = (typeIndexL + (typeIndexR - typeIndexL) / 2) * size;
        printf("m = %zu\n", m);

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
    printf("merge\n");
    printf("l = %zu, m = %zu, r = %zu, size = %zu\n", l, m, r, size);
    // calculate size of temp arrays
    size_t nL = m - l;
    size_t nR = r - m + 1;
    printf("nL = %zu, nR = %zu\n", nL, nR);

    // sub array index iterators 
    size_t mergeIndex = l, leftIndex = 0, rightIndex = 0;

    // initialize and populate temp arrays
    uint8_t leftArr[nL];
    uint8_t rightArr[nR];
    uint8_t* baseArr = ((uint8_t*) base);

    for (size_t i = 0; i < nL; ++i) {
        leftArr[i] = baseArr[l + i];
    }
    for (size_t i = 0; i < nR; ++i) {
        rightArr[i] = baseArr[m + i];
    }
    
    // merge left and right arrays in order 
    while (leftIndex < nL && rightIndex < nR) {
        if (compar(&leftArr[leftIndex], &rightArr[rightIndex]) < 1) {
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
    printf("compare_int: ");
    int const* A = a;
    int const* B = b;
    printf("*A = %d, *B = %d\n", *A, *B);
    if (*A < *B) return -1;
    else if (*A > *B) return +1;
    else return 0;
}

int compare_string(void const* a, void const* b) {
    uint8_t arrA[sizeof(char*)];
    uint8_t arrB[sizeof(char*)];
    
    char* arra = (char*) a;
    char* arrb = (char*) b;
    
    for (size_t i = 0; i < sizeof(char*); ++i) {
        arrA[i] = arra[i];
    }
    for (size_t i = 0; i < sizeof(char*); ++i) {
        arrA[i] = arrb[i];
    }

    char* strA = (char*) arrA;
    char* strB = (char*) arrB;

    int ret = strcmp(strA, strB);
    printf("compare_string: *A = %s, *B = %s, ret = %d\n", strA, strB, ret);
    return ret;
}

