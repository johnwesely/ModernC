// exercise 1, 2, and 3 from Mordern C, Part 2, chapter 12.3
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

typedef union unsignedInspect unsignedInspect;
typedef union doublePointer doublePointer;

union unsignedInspect {
    unsigned val;
    unsigned char bytes[sizeof(unsigned)];
};

union doublePointer {
    unsigned val;
    unsigned char bytes[sizeof(double*)];
};

int main(void) {
    unsignedInspect twofold = { .val = 0xAABBCCDD, };
    printf("value is 0x%.08X\n", twofold.val);
    for (size_t i = 0; i < sizeof(twofold.bytes); ++i) {
        printf("byte[%zu]: 0x%.02hhX\n", i, twofold.bytes[i]);
    }
    
    double j = 1000;
    double* p = &j;
    doublePointer dp = { .val = &p, };

    printf("value of dp is 0x%.08X\n", dp.val);

    double n[] = { 0, 1, 2 };

    doublePointer index1 = { .val = &n[1], };
    doublePointer index2 = { .val = &n[2], };

    printf("value of index1 is 0x%.08X\nvalue of index2 is 0x%.08X\n", index1.val, index2.val);
}