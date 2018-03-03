#ifndef _COMMON_H_
#define _COMMON_H_

#include <unistd.h>
#include <curses.h>

// TODO REMOVE ITMM CODE
void warn(char *msg);
void panic(char *msg);


#define msleep(a) usleep((a)*1000)
#define ARRAY_LEN(a) (sizeof(a) / sizeof(*a))

#define BITMASK_LSB(type, bitnum) ( ( (type) -1LL) >> ( (sizeof(type)*8) - (bitnum)) )

WINDOW* DEBUG_WINDOW;

#ifdef DEBUG
#define DEBUG_MPRINTF(y, x, s, ...) { attrset(A_NORMAL); mvwprintw(DEBUG_WINDOW, y, x, s, __VA_ARGS__); wrefresh(DEBUG_WINDOW);}
#define DEBUG_MPRINT(y, x, s) { attrset(A_NORMAL); mvwaddstr(DEBUG_WINDOW, y, x, s); wrefresh(DEBUG_WINDOW);}
#define DEBUG_PRINTF(s, ...) { attrset(A_NORMAL); wprintw(DEBUG_WINDOW, s, __VA_ARGS__); wrefresh(DEBUG_WINDOW);}
#define DEBUG_PRINT(s) { attrset(A_NORMAL); waddstr(DEBUG_WINDOW, s); wrefresh(DEBUG_WINDOW);}
#else
// TODO: how to just omit this shit
#define DEBUG_MPRINTF(y, x, s, ...) { (void)y, (void)x, (void)s, (void)__VA_ARGS__; }
#define DEBUG_MPRINT(y, x, s, ...) { (void)y, (void)x, (void)s, (void)__VA_ARGS__; }
#endif

int log2c( unsigned int n );

#endif /* _COMMON_H_  */
