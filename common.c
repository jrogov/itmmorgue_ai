#include "stdlib.h"

#include "common.h"
// Ceiled logarithm of integer number

void warn(char *msg) {
    if (msg) {
        fprintf(stderr, "%s\n", msg);
    }
}

void panic(char *msg) {
    endwin();

    warn(msg);

    exit(EXIT_FAILURE);
}


int log2c( unsigned int n ){
    
    // int i;
    // for( i=0; 0 != n; ++i)
    //     n>>=1;
    // return i;

    return ( n <= 1 )? 0 : (sizeof(typeof(n))<<3) - __builtin_clz(n-1);
}

#ifdef DEBUG

WINDOW* DEBUG_WINDOW;
int DEBUG_STARTPOS_Y;
int DEBUG_STARTPOS_X;
void null_varargs(int i, ...){(void) i;}

#endif
