/*
** takes 3 command line arguments of type int
** assigns to three variables and 
** shifts the values of these objects cyclically 
*/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

void cycleShift(int* a, int* b, int* c) {
    int tmp0 = *a;
    int tmp1 = *b;

    *a = *c;
    *b = tmp0;
    *c = tmp1;
}

int main(int argc, char* argv[argc + 1]) {
    int ret = EXIT_FAILURE;
    assert (argc == 4);

    int x = atoi(argv[1]);
    int y = atoi(argv[2]);
    int z = atoi(argv[3]);

    printf("x = %d, y = %d, z= %d\nAfter shifting:\n", x, y, z);

    cycleShift(&x, &y, &z);

    printf("x = %d, y = %d, z = %d\n", x, y, z);
    ret = EXIT_SUCCESS;
    
    return ret;
}