/* Challenge Problem 12 from Modern C
** processes text into a doubly linked list
** takes one command line argument, a text file
** to be processed */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct blob blob;
struct blob {
    char* str;
    blob* next;
    blob* previous;
};

enum { buf_max = 80 };

/* makes a new object of type blob with given 
** string and returns pointer to blob */
blob* makeBlob(char* str);    
/* appends blob to end of list of blobs */
void appendBlob(blob* blobList, blob* blb);
/* inserts blob into list of blobs 
** between two given blobs */
void insertBlob(blob* blb, blob* next, blob* previous);
/* splits one blob into two blobs at given point */
blob* splitBlob(blob* blb, size_t splitPoint);
/* joins two given blobs into one */
blob* joinBlobs(blob* blobA, blob* blobB);
/* deletes given blob from list
** joins previous and next blob together **/ 
void deleteBlob(blob* blb);
/* reads a text and puts each line into a blob */
blob* text2Blob(FILE* text);
/* prints a list of blobs */
void printBlob(blob* blb);

int main(int argc, char* argv[argc + 1]) {
   FILE* txt = fopen(argv[1], "r");
   if (txt) {
       blob* b1 = text2Blob(txt);
       printBlob(b1);
   }
   else printf("file load error.\n");
}

blob* makeBlob(char* str) {
    blob* new = malloc(sizeof(str) + sizeof(char*) * 2); 
    new -> str = str;
    new -> next = 0;
    new -> previous = 0;
    return new; 
}

void appendBlob(blob* blobList, blob* blb) {
    while (blobList -> next != 0) {
        blobList = blobList -> next;
    }
    blobList -> next = blb;
    blb -> previous = blobList;
}

void insertBlob(blob* blb, blob* next, blob* previous) {
    previous -> next = blb;
    next -> previous = blb;
    blb -> next = next;
    blb -> previous = previous;
}

blob* splitBlob(blob* blb, size_t splitPoint) {
    if (splitPoint == 0 || splitPoint > strlen(blb -> str) - 1) {
        perror("splitPoint outside bounds of blb -> str");
        return blb;
    }
    char* strInit = blb -> str;
    size_t len = strlen(strInit);
    // initialize sub strings
    char* strA = malloc(sizeof(char) * (splitPoint + 1));
    char* strB = malloc(sizeof(char) * (len - splitPoint + 1));

    // populate sub arrays
    for (size_t i = 0; i < len; ++i) {
        if (i < splitPoint) {
            strA[i] = strInit[i];
            if (i == splitPoint - 1) strA[splitPoint] = 0;
        }
        else {
            strB[i - splitPoint] = strInit[i];
            if (i == len - 1) strB[i + 1] = 0;
        }
    }

    // insert two new sub blobs into list in place of blb
    blob* blbA = makeBlob(strA);
    blob* blbB = makeBlob(strB);

    blbA -> previous = blb -> previous;
    blbA -> next = blbB;
    blbB -> previous = blbA;
    blbB -> next = blb -> next;
    // only set values of next and previous blobs
    // if such blobs exist
    if (blb -> previous) blb -> previous -> next = blbA;
    if (blb -> next) blb -> next -> previous = blbB;
    free(blb);
    return blbA; // returns blbA incase splitBlob is applied to head of list
}

blob* joinBlobs(blob* blobA, blob* blobB) {
    size_t lenA = strlen(blobA -> str);
    size_t lenB = strlen(blobB -> str);
    // concatenates strings from blobA and blobB
    char* strAB = malloc(sizeof(char) * (lenA + lenB + 1));

    for (size_t i = 0; i < lenA; ++i) {
        strAB[i] = (blobA -> str)[i];
    }

    for (size_t i = lenA, j = 0; j < lenB; ++j, ++i) {
        strAB[i] = (blobB -> str)[j];
    }

    strAB[lenA + lenB] = 0;
    // create new blob and reroute pointers
    blob* blobNew = makeBlob(strAB);

    blobNew -> previous = blobA -> previous;
    blobNew -> next = blobB -> next;
    if (blobA -> previous) blobA -> previous -> next = blobNew;
    if (blobB -> next) blobB -> next -> previous = blobNew;

    free(blobA);
    free(blobB);
    return blobNew;
}

blob* text2Blob(FILE* file) {
    blob* head = makeBlob("");
    blob* prev = head;
    char* buffer = malloc(sizeof(char) *  buf_max); 

    while (fgets(buffer, buf_max, file)) {
        blob* current = makeBlob(buffer);
        current -> previous = prev;
        prev -> next = current;
        prev = current;
        buffer = malloc(sizeof(char) * buf_max);
    }

    return head;
}

void printBlob(blob* blb) {
    if (blb -> next) {
        printf("%s", blb -> str);
        printBlob(blb -> next);
    }
    else {
        printf("%s", blb -> str);
    }
}