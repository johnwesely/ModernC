// Exercise 1:8.4:6 from Modern C
// concatenates files given as command line arguments to stdout
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

enum { buf_max = 32};

int main(int argc, char* argv[argc + 1]) {
    int ret = EXIT_FAILURE;
    char buffer[buf_max] = { 0 };
    
    // if no arguments are given read from stdin
    if (argc == 1) {
        while (fgets(buffer, buf_max, stdin)) {
            fputs(buffer, stdout);
        }
        ret = EXIT_SUCCESS; 
    }

    // open each file in argv and print to stdout 
    // if file cannot be opened, print error message and continue to next file
    for (int i = 1; i < argc; ++i) {
        FILE* instream = fopen(argv[i], "r");
        if (instream) {
            while (fgets(buffer, buf_max, instream)) {
                fputs(buffer, stdout);
            }
            fclose(instream);
            ret = EXIT_SUCCESS;
        }
        else {
            fprintf(stderr, "could not open %s: ", argv[i]);
            perror(0);
            errno = 0;
        }
    }
    return ret;
}