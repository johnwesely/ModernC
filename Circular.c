// implementation of a dynamic sized circular buffer data structure
// exercises 2 and 3 from Modern C, Part 2, Chapter 13
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct circular circular;

struct circular {
    size_t start;
    size_t len;
    size_t maxLen;
    double* tab;
};

/* append a new element with the given value to c
** returns 0 if unsuccessful */
circular* circularAppend(circular* c, double value);

/* removes the oldest element from c 
** returns value of popped element or 0.0 c has no elements */
double circularPop(circular* c);

/* returns a pointer to position pos in c
** returns 0 if no element exists at pos */
double* circularElement(circular* c, size_t pos);

/* initializes a prexisting buffer, c, with max_len elements */
circular* circularInit(circular* c, size_t maxLen);

/* destroy given buffer initialized by circularInit */
void circularDestroy(circular* c);

/* allocates and initilizes a circular buffer of given len */
circular* circularNew(size_t len);

/* deletes circular buffer created with circularNew */
void circularDelete(circular* c);

/* resize capacity of buffer c to max_len */
circular* circularResize(circular* c, size_t maxLen);

/* helper function to get elements positions in circular array */
static size_t circularGetPos(circular* c, size_t pos);

/* prints circular buffer */
void printBuffer(circular* c);

int main(void) {
    circular *c = circularNew(4);

    circularAppend(c, 4.2);
    circularAppend(c, 6.9);
    circularAppend(c, 9.42);

    printf("oldest value is %.2lf\n", circularPop(c));
     
    circularAppend(c, 1.2);
    circularAppend(c, 3.6);
    circularPop(c);
    printBuffer(c);


    printf("length is %zu\n", c->len);
    circularResize(c, 10);

    printBuffer(c);

    circularResize(c, 3);
    printBuffer(c);

    circularResize(c, 10);
    printBuffer(c);
    return 1;
}

circular* circularAppend(circular* c, double value) {
    if (c->len >= c->maxLen) return 0;
    size_t pos = circularGetPos(c , c->len);
    c->tab[pos] = value;
    ++c->len;
    return c;
}

double circularPop(circular* c) {
    double ret = 0.0;
    if (!c->len) return ret;
    ret = c->tab[c->start];
    c->tab[c->start] = 0;
    c->start = (c->start + 1) % c->maxLen;
    --c->len;
    return ret;
}

circular* circularNew(size_t len) {
    circular* c = malloc(sizeof(circular));
    *c = (circular) {
        .maxLen = len,
        .start = 0,
        .len = 0,
        .tab = malloc(sizeof(double) * len),
    };
    return c;
}

void circularDelete(circular* c) {
    free(c);
}

circular* circularInit(circular* c, size_t maxLen) {
    if (c) {
        if (maxLen) {
            *c = (circular) {
                .maxLen = maxLen,
                .tab = malloc(sizeof(double[maxLen])),
            };
            // failed allocation
            if (!c -> tab) c -> maxLen = 0;
        }
        else {
            // empty, zero length buffer
            *c = (circular) { 0 };
        }
    }
    return c;
}

void circularDestroy(circular* c) {
    if (c) {
        // free tab array
        free(c -> tab);
        // initialize c to empty, zero length buffer
        circularInit(c, 0);
    }
}

static size_t circularGetPos(circular* c, size_t pos) {
    pos += c -> start;
    pos %= c -> maxLen;
    return pos;
}

double* circularElement(circular* c, size_t pos) {
    double* ret = 0;
    if (c) {
        if (pos < c -> maxLen) {
            // gets pos from beggining of circular array
            pos = circularGetPos(c, pos);
            // address of element in circular array
            ret = &c -> tab[pos];
        }
    }
    return ret;
}

circular* circularResize(circular* c, size_t newLen) {
    if (c) {
        size_t len = c->len;
        if (len > newLen) return 0;
        size_t oldLen = c->maxLen;
        if (newLen != oldLen) {
            size_t oldStart = circularGetPos(c, 0);
            size_t newStart = oldStart;
            double* newTab; 
            double* oldTab = c->tab;
            // if growing buffer
            if (newLen > oldLen) {
                newTab = realloc(c->tab, sizeof(double[newLen]));
                if (!newTab) return 0;
                // if buffer is wrapped 
                if (oldStart + len > oldLen) {
                    // upper Length
                    size_t upperLen = oldLen - oldStart;
                    // lower Length
                    size_t lowerLen = len - upperLen;
                    // if the lower chunk will fit at end of new arr
                    if (lowerLen <= (newLen - oldLen)) {
                        memcpy(newTab + oldLen, newTab, 
                        lowerLen*sizeof(double));
                    }
                    // if not, move upper chunk to end of new arr
                    else {
                        newStart = newLen - upperLen;
                        memmove(newTab + newStart, newTab + oldStart,
                        upperLen * sizeof(double));
                    }
                }
            }
            // if shrinking buffer
            else {
                if (len < newLen) return 0;
                newTab = malloc(sizeof(double[newLen]));
                if (!newTab) return 0;
                // if buffer is wrapped
                if (oldStart + len > oldLen) {
                    size_t upperLen = oldLen - oldStart;
                    size_t lowerLen = len - upperLen;
                    // move upper chunk
                    memcpy(newTab, oldTab + oldStart,
                    sizeof(double) * upperLen);
                    // move lower chunk
                    memcpy(newTab + upperLen, oldTab,
                    sizeof(double) * lowerLen);
                }
                else {
                    memcpy(newTab, oldTab + oldStart,
                    sizeof(double) * len);
                }
                newStart = 0;
                free(oldTab);
            }
            *c = (circular) {
                .maxLen = newLen,
                .start = newStart,
                .len = len,
                .tab = newTab,
                };
            }
        }
    return c;
}

void printBuffer(circular* c) {
    printf("index:   ");
    for (size_t i = 0; i < c->maxLen; ++i) {
        printf("%zu    ", i);
    }
    printf("\nvalue: ");
    for (size_t i = 0; i < c->maxLen; ++i) {
        printf("%.2lf ", c->tab[i]);
    }
    printf("\n");
}