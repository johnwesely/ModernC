// implementation of mergeSort from challenge problem 1
#include <stdlib.h>
#include <stdio.h>

void merge(int arr[], int l, int m, int r) {
  // temp array sizes
  int nL = m - l + 1;
  int nR = r - m;
  
  // sub array index iterator 
  size_t mergeIndex = l, leftIndex = 0, rightIndex = 0;

  // temporary arrays
  int Left[nL];
  int Right[nR];

  // populate temp arrays with sub arrays
  for (size_t i = 0; i < nL; ++i) {
      Left[i] = arr[l + i];
  }
  for (size_t i = 0; i < nR; ++i) {
      Right[i] = arr[m + 1 + i];
  }

  // sort temp arrays bacrightIndex into main array
  while (leftIndex < nL && rightIndex < nR) { 
      if (Left[leftIndex] <= Right[rightIndex]) {
          arr[mergeIndex] = Left[leftIndex];
          ++leftIndex; 
      }
      else {
          arr[mergeIndex] = Right[rightIndex];
          ++rightIndex;
      }
      ++mergeIndex;
  }

  // insert last remaining value to end of sub array
  while (rightIndex < nR) {
      arr[mergeIndex] = Right[rightIndex];
      ++mergeIndex;
      ++rightIndex;
  }
  while (leftIndex < nL) {
      arr[mergeIndex] = Left[leftIndex];
      ++mergeIndex;
      ++leftIndex;
  }
}

void mergeSort(int arr[], int l, int r) {
  // base case
  if (l < r) {
      // middle index of sub array 
      int m = l + (r - l) / 2;

      // sort Left and right halves of sub array
      mergeSort(arr, l, m);
      mergeSort(arr, m + 1, r);

      // merge sorted halves
      merge(arr, l, m, r);
  }
}

int main(void) {
  int arr[] = {4, 1, 3, 2};
  printf("unsorted array\n");
  for (size_t i = 0; i < 4; ++i) {
      printf("%d\n", arr[i]);
  }

  mergeSort(arr, 0, 3);

  // print sorted array
  printf("sorted array\n");
  for (size_t i = 0; i < 4; ++i) {
      printf("%d\n", arr[i]);
  }  

  return 0;
}