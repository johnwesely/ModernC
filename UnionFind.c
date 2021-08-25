// algorithm for quick find, slow union from Challenge problem 4

#include <stdio.h>
#include <stdlib.h>

size_t SIZE_MAX = -1;
size_t size = 10;

// Join two sets
void Union(size_t arr[], size_t nodeA, size_t nodeB);
// find root of set
size_t find(size_t arr[], size_t node);
// changes all entries on path to root to val
void findReplace(size_t arr[], size_t node, size_t val);
//  compresses set to point directly to root
void findCompress(size_t arr[], size_t node); 
// compress helper function 
void compress(size_t arr[], size_t node, size_t val);
// initializes parent array of size to SIZE_MAX
void initializeUF(size_t arr[], size_t size);

int main(void) {
    size_t size = 10;
    size_t parent[size];
    initializeUF(parent, size);
    Union(parent, 1, 2);
    Union(parent, 0, 3);
    Union(parent, 0, 1);
    Union(parent, 4, 5);
    Union(parent, 5, 6);
    Union(parent, 6, 7);
    Union(parent, 7, 8);
    Union(parent, 8, 9);
    Union(parent, 1, 9);
    Union(parent, 4, 9);
    Union(parent, 5, 4);
    //findReplace(parent, 4, 2);
    //findCompress(parent, 4);

    for (size_t i = 0; i < size; ++i) {
        printf("%zu\n", parent[i]);
    }

}

void initializeUF(size_t arr[], size_t size) {
    for (size_t i = 0; i < size; ++i) {
        arr[i] = SIZE_MAX;
    }
}

size_t find(size_t arr[], size_t node) {
    while (arr[node] != SIZE_MAX) {
        node = arr[node];
    }
    return node;
}

void Union(size_t arr[], size_t nodeA, size_t nodeB) {
    size_t root = find(arr, nodeA);
    findCompress(arr, nodeA);
    // if both nodes share a root, compress tree b, 
    // else replace all values of tree b with the root of a
    if (root == find(arr, nodeB)) {
        findCompress(arr, nodeB);
    }
    else {
        findReplace(arr, nodeB, root);
    }
}

void findReplace(size_t arr[], size_t node, size_t val) {
    if (node == SIZE_MAX) {
        return;
    }
    size_t next = arr[node];
    arr[node] = val;
    findReplace(arr, next, val);
}

void findCompress(size_t arr[], size_t node) {
    size_t root = find(arr, node);
    compress(arr, node, root);
}

void compress(size_t arr[], size_t node, size_t val) {
    if (arr[node] == SIZE_MAX) {
        return;
    }
    size_t next = arr[node];
    arr[node] = val;
    compress(arr, next, val);
}