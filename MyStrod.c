// transforms argv[1] into floating point double
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

double myStrod(const char *str);

int main(int argc, char *argv[]){
    printf("%f\n", myStrod(argv[1]));
    return 0;
}

double myStrod(const char *str) {
    size_t int_len = 0;
    double dbl = 0.0;

    while (str[int_len] != '.' && str[int_len] != 0) {
        ++int_len;
    }
    
    // string index 
    size_t strIndex = 0;
    // decimal place
    int decimalPlace = int_len - 1;
    // while end condition 
    int end = decimalPlace - strlen(str) + 1;
    
    while (decimalPlace > end) {
        // if str[j] != '.'
        if (strIndex != int_len) {
            dbl += (str[strIndex] - 48) * pow(10, decimalPlace);
        }
        else {
            ++decimalPlace;
        }
        --decimalPlace;
        ++strIndex;
    }

    return dbl;
}