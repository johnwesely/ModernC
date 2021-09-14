#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

/* searchs for needle in hay 
** returns index of needle in hay or 0 if not found */
size_t wordSearch(char* needle, char* hay);

/* returns true is needle == hay[i] */
size_t checkMatch(char* needle, char* hay, size_t i);

/* returns memory allocated string 
** with first incidence of needle in hay replaced with new */
char* searchReplace(char* needle, char* new, char* hay);

/* searches for regular expression needle in string hay
** returns index of needle within hay */
size_t regExSearch(char* needle, char* hay);

/* sets charTable values to neg for a given range */
void setRange(char start, char end, size_t neg);

/* returns true is hayChar matches regular expression at needIndex */
size_t checkChar(char* needle, char hayChar, size_t needIndex);

/* returns end of match index for hay is regular expression needle
** matches hay[i] */
size_t checkRegExMatch(char* needle, char* hay, size_t i);

/* searches for regular expression needle in hay
** and returns memory allocated with with needle replaced by new */
char* regExSearchReplace(char* needle, char* new, char* hay);

/* represents a regular expression pattern in format '[...]' in charTable */
void readPattern(char* pat); 

/* prints charTable for debug purposes */
void printCharTable(void);


/* each index corresponds to ASCII char
** value is 1 for matching, 0 for not matching */
static uint8_t charTable[127] = { 0 };

int main(void) {
    printf("%zu\n", regExSearch("s*[l-z]as", "apples taste really bad"));
    printf("%s\n", regExSearchReplace("b*d", "great", "apples taste really bad"));
    printf("%s\n", regExSearchReplace("[a-z]*te", "are", "apples taste really bad"));
    printf("%s\n", regExSearchReplace("r?x[a-z][a-z]l*y", "extremely", "apples taste really bad"));
    return 1;
}

size_t wordSearch(char* needle, char* hay) {
    size_t found = 0; 
    size_t needLen = strlen(needle);

    for (size_t i = 0; i < (strlen(hay) - needLen + 1); ++i) {
        if (needle[0] == hay[i]) {
            found = checkMatch(needle, hay, i); 
        }
        if (found) return i;
    }
    return 0;
}

size_t checkMatch(char* needle, char* hay, size_t i) {
    size_t j = 0;
    while (needle[j]) {
        if (hay[i] == needle[j]) {
            ++i; ++j;
        }
        else return 0;
    }
    return 1;
}

char* searchReplace(char* needle, char* new, char* hay) {
    size_t start = wordSearch(needle, hay);
    if (start) {
        size_t newLen = strlen(new);
        size_t needLen = strlen(needle);
        size_t hayLen = strlen(hay);
        int diff = needLen - newLen;
        char* ret = malloc(sizeof(char) * hayLen - diff);
        // copy new text to return string
        memcpy(ret, hay, start);
        memcpy(ret + start, new, newLen);
        memcpy(ret + start + newLen, hay + needLen + start, hayLen - start - needLen);
        return ret;
    }
    return hay; 
}

/* helper function for regExSearch
** returns a 3 element size_t array
** ret[0] = starting hay index of match
** ret[1] = ending hay index of match
** ret[2] = true if match is found */
size_t* regExSearchH(char* needle, char* hay) {
    size_t found = 0;
    size_t* ret = malloc(sizeof(size_t) * 3);
    ret[0] = 0; ret[1] = 0; ret[2] = 0;

    for (size_t i = 0; i < strlen(hay); ++i) {
        if (checkChar(needle, hay[i], 0)) {
            found = checkRegExMatch(needle, hay, i);
        }
        if (found) {
            ret[0] = i; ret[1] = found; ret[2] = 1;
            return ret;
        }
    }
    return ret;
}

size_t regExSearch(char* needle, char* hay) {
    size_t* search = regExSearchH(needle, hay);
    if (!search[2]) printf("Reg Ex: %s not found\n", needle);
    size_t ret = search[0];
    free(search);
    return ret;
}

size_t checkChar(char* needle, char hayChar, size_t needIndex) {
    if (needle[needIndex] == '[' /*&& needle[needIndex - 1] != 92*/) {
        readPattern(needle + needIndex);
        for (size_t i = 0; i < 128; ++i) {
            if (charTable[i] && i == hayChar) {
                return 1;
            }
        }
    }
    else {
        if (needle[needIndex] == hayChar) {
            return 1;
        }
    }
    return 0;
}

size_t checkRegExMatch(char* needle, char* hay, size_t i) {
    size_t j = 0;
    while (needle[j]) {
        // matches pattern
        if (needle[j] == '[') {
            if (!checkChar(needle, hay[i], j)) {
                return 0;
            }
            // increment j until end of pattern
            while (needle[j] != ']') ++j;
            ++j; ++i;
        }
        // matches zero or one of the next regex
        else if (needle[j] == '?') {
            if (checkChar(needle, hay[i], j+1)) ++i;
            if (needle[j+1] == '[') {
                while (needle[j] != ']') ++j;
                ++j;
            }
            else j += 2;
        }
        // greedily matches anything
        else if (needle[j] == '*') {
            ++j;
            while (hay[i] && !checkChar(needle, hay[i], j)) {
                ++i;
            }
            if (!hay[i]) return 0;
            if (needle[j] == '[') {
                while (needle[j] != ']') ++j;
            }
            ++j;
            ++i;
        }
        // matches character
        else {
            if (!checkChar(needle, hay[i], j)) {
                return 0;
            }
            ++j; ++i;
        }
    }
    return i;
}

void readPattern(char* pat) {
    // reset charTable
    memset(charTable, 0, 128);
    // variables for if pattern contains a range
    char rangeStart = 0;
    char rangeEnd = 0;
    size_t neg = 1;
    
    if (pat[1] == '^') {
        neg = 0;
        memset(charTable, 1, 128);
    }

    for (size_t i = 1; pat[i] != ']' && pat[i-1] != '\\'; ++i) {
        // negation carrot
        if (pat[i] == '^' && pat[i-1] != '\\') {
            continue; 
        }
        // single character 
        else if (pat[i+1] != '-') {
            charTable[(size_t)pat[i]] = neg;
        }
        // range
        else {
            rangeStart = pat[i];
            rangeEnd = pat[i+2];
            setRange(rangeStart, rangeEnd, neg);
            i += 2;
        }
    }
}

char* regExSearchReplace(char* needle, char* new, char* hay) {
    size_t* search = regExSearchH(needle, hay);
    if (!search[2]) return hay;

    size_t start = search[0];
    size_t end = search[1]; 
    size_t newLen = strlen(new);
    size_t needLen = end - start;
    size_t hayLen = strlen(hay);   
    int diff = needLen - newLen;
    char* ret = malloc(sizeof(char) * hayLen - diff);
    // copy new text to return string
    memcpy(ret, hay, start);
    memcpy(ret + start, new, newLen);
    memcpy(ret + start +  newLen,
            hay + start + needLen,
            hayLen - start - needLen);
    return ret;
}


void setRange(char start, char end, size_t neg) {
    // if end is less than start, swap values
    if (end < start) {
        start = start + end;
        end = start - end;
        start = start - end;
    }
    for (size_t i = start; i < end + 1; ++i) {
        charTable[i] = neg;
    }
}

void printCharTable(void) {
    printf("charTable:\n");
    for (size_t i = 0; i < 128; ++i) {
        printf("%c = %d\n", (char) i, charTable[i]);
    }
}