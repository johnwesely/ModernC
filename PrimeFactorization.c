// Challenge problem 9 from Modern C
// prints prime factors of all command line arguments 
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

size_t isDivisible(size_t num, size_t denom);
size_t isEven(size_t n);
void printPrimeFactors(size_t n);

int main(int argc, char *argv[]) {
    for (size_t i = 1; i < argc; i++) {
        printPrimeFactors(atoi(argv[i]));
    }
    return 1;
}

// helper function to deternime if numerator is divisible by denonminator
// with no remainder 
size_t isDivisible(size_t num, size_t denom) {
    return (num % denom == 0);
}

// helper function returns wheter n is even
size_t isEven(size_t n) {
    return isDivisible(n, 2);
}

// prints all prime factors of n
void printPrimeFactors(size_t n) {
    printf("The prime factors of %zu are: ", n);

    while (isEven(n)) {
        printf("%d, ", 2);
        n = n / 2;
    }

    for (size_t i = 3; i < round(sqrt(n)) + 1; i += 2) {
        while (isDivisible(n , i)) {
            printf("%zu, ", i);
            n = n / i;
        }
    }

    if (n > 2) {
        printf("%zu\n", n);
    }
    else {
        printf("\b\b \n");
    }
}