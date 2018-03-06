#ifndef _COMMON_H_
#define _COMMON_H_

#include <unistd.h>
#include <curses.h>

// TODO REMOVE ITMM CODE
void warn(char *msg);
void panic(char *msg);


#define msleep(a) usleep((a)*1000)
#define ARRAY_LEN(a) (sizeof(a) / sizeof(*a))

#define BITMASK_LSB(type, bitnum) ( ( (type) -1ULL) >> ( (sizeof(type)*8) - (bitnum)) )


#ifdef DEBUG

extern WINDOW* DEBUG_WINDOW;
extern int DEBUG_STARTPOS_Y;
extern int DEBUG_STARTPOS_X;

#define DEBUG_INIT(height, width, y, x) do {                                                            \
                                            DEBUG_WINDOW = newwin( (height), (width), (y), (x) );       \
                                            if( NULL == DEBUG_WINDOW )                                  \
                                                panic("Couldn't create Debug window");                  \
                                            DEBUG_STARTPOS_Y = 0;                                       \
                                            DEBUG_STARTPOS_X = 0;                                       \
                                        } while(0)

// Fix position for DEBUG_RESET to current cursor position                                        
#define DEBUG_FIXPOS()                  do {                                                            \
                                            getyx(DEBUG_WINDOW, DEBUG_STARTPOS_Y, DEBUG_STARTPOS_X);    \
                                        } while(0)

// Fix position for DEBUG_RESET to given position                                        
#define DEBUG_STARTPOS(y, x)            do {                                                            \
                                            DEBUG_STARTPOS_Y = (y);                                     \
                                            DEBUG_STARTPOS_X = (x);                                     \
                                        } while(0)

// Reset cursor to "default" position (DEBUG_STARTPOS_[YX])
#define DEBUG_RESET()                   do {                                                            \
                                             wmove(DEBUG_WINDOW, DEBUG_STARTPOS_Y, DEBUG_STARTPOS_X);   \
                                        } while(0)

// Immediate-refresh print
#define DEBUG_IMPRINTF(y, x, s, ...)    do {                                                            \
                                            wattrset(DEBUG_WINDOW, A_NORMAL);                           \
                                            mvwprintw(DEBUG_WINDOW, y, x, s, __VA_ARGS__);              \
                                            wrefresh(DEBUG_WINDOW);                                     \
                                        } while(0)

#define DEBUG_IMPRINT(y, x, s)          do {                                                            \
                                            wattrset(DEBUG_WINDOW, A_NORMAL);                           \
                                            mvwaddstr(DEBUG_WINDOW, y, x, s);                           \
                                            wrefresh(DEBUG_WINDOW);                                     \
                                        } while(0)

// Postponed-refresh print: call DEBUG_REFRESH after that
#define DEBUG_MPRINTF(y, x, s, ...)     do {                                                            \
                                            wattrset(DEBUG_WINDOW, A_NORMAL);                           \
                                            mvwprintw(DEBUG_WINDOW, y, x, s, __VA_ARGS__);              \
                                        } while(0)

#define DEBUG_MPRINT(y, x, s)           do {                                                            \
                                            wattrset(DEBUG_WINDOW, A_NORMAL);                           \
                                            mvwaddstr(DEBUG_WINDOW, y, x, s);                           \
                                        } while(0)


#define DEBUG_PRINTF(s, ...)            do {                                                            \
                                            wattrset(DEBUG_WINDOW, A_NORMAL);                           \
                                            wprintw(DEBUG_WINDOW, s, __VA_ARGS__);                      \
                                        } while(0)

#define DEBUG_PRINT(s)                  do {                                                            \
                                            wattrset(DEBUG_WINDOW, A_NORMAL);                           \
                                            waddstr(DEBUG_WINDOW, s);                                   \
                                        } while(0)


#define DEBUG_REFRESH()                 do {                                                            \
                                            wrefresh(DEBUG_WINDOW);                                     \
                                        } while(0)


#define DEBUG_WAIT_PRESS()               wgetch(DEBUG_WINDOW)

#else

// I will be beaten up for this
void null_varargs(int i, ...);
#define void_varargs(...) if(0){ null_varargs(0, __VA_ARGS__);};

// A pack of stubs that does nothing
#define DEBUG_DO_NOTHING()                      do{} while(0)                                                        
#define DEBUG_DO_NOTHING_UNUSED_1(a)                  do{ (void)(a); } while(0)                                            
#define DEBUG_DO_NOTHING_UNUSED_2(a,b)                do{ (void)(a); (void)(b); } while(0)                                 
#define DEBUG_DO_NOTHING_UNUSED_3(a,b,c)              do{ (void)(a); (void)(b); (void)(c); } while(0)                      
#define DEBUG_DO_NOTHING_UNUSED_4(a,b,c,d)            do{ (void)(a); (void)(b); (void)(c); (void)(d); } while(0)                      
#define DEBUG_DO_NOTHING_UNUSED_1_VA(a, ...)          do{ DEBUG_DO_NOTHING_UNUSED_1(a); void_varargs(__VA_ARGS__); } while(0)    
#define DEBUG_DO_NOTHING_UNUSED_2_VA(a, b, ...)       do{ DEBUG_DO_NOTHING_UNUSED_2(a,b); void_varargs(__VA_ARGS__); } while(0)  
#define DEBUG_DO_NOTHING_UNUSED_3_VA(a, b, c, ...)    do{ DEBUG_DO_NOTHING_UNUSED_3(a,b,c); void_varargs(__VA_ARGS__); } while(0)
#define DEBUG_DO_NOTHING_UNUSED_4_VA(a, b, c, d, ...) do{ DEBUG_DO_NOTHING_UNUSED_3(a,b,c,d); void_varargs(__VA_ARGS__); } while(0)


#define DEBUG_INIT(height, width, y, x) DEBUG_DO_NOTHING_UNUSED_4((height),(width),(y),(x))
#define DEBUG_FIXPOS()                  DEBUG_DO_NOTHING()
#define DEBUG_STARTPOS(y, x)            DEBUG_DO_NOTHING_UNUSED_2((y),(x))
#define DEBUG_RESET()                   DEBUG_DO_NOTHING()
#define DEBUG_IMPRINTF(y, x, s, ...)    DEBUG_DO_NOTHING_UNUSED_3_VA((y),(x),(s), __VA_ARGS__)
#define DEBUG_IMPRINT(y, x, s)          DEBUG_DO_NOTHING_UNUSED_3_VA((y),(x),(s))
#define DEBUG_MPRINTF(y, x, s, ...)     DEBUG_DO_NOTHING_UNUSED_3_VA((y),(x),(s), __VA_ARGS__)
#define DEBUG_MPRINT(y, x, s)           DEBUG_DO_NOTHING_UNUSED_3_VA((y),(x),(s))
#define DEBUG_PRINTF(s, ...)            DEBUG_DO_NOTHING_UNUSED_1_VA((s), __VA_ARGS__)
#define DEBUG_PRINT(s)                  DEBUG_DO_NOTHING_UNUSED_1(s)
#define DEBUG_REFRESH()                 DEBUG_DO_NOTHING()
#define DEBUG_WAIT_PRESS()              DEBUG_DO_NOTHING()

#endif /* DEBUG */

int log2c( unsigned int n );

#endif /* _COMMON_H_  */
