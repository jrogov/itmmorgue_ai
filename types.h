#ifndef _TYPES_H_
#define _TYPES_H_

#include <stdint.h>
#include <stdlib.h>
#include <stdlib.h>

// Useful Macroses

#define PTR_OFFSET(a, b) ( ((char*)(a)) + (b) )


// Basic types

typedef int32_t entid_t;
typedef uint32_t tick_t;
typedef uint16_t pos_t;     // MUST BE UNSIGNED 
typedef uint16_t len_t;
typedef struct coordinates vec2_t;

struct coordinates {
    pos_t y;
    pos_t x;
};

#define SVEC2(a,b) { .y=(a), .x=(b) }
#define VEC2(a,b) (vec2_t) { .y=(a), .x=(b) } 

// 2D VECTOR OPERATIONS
#define VEC2_MDIST(a,b)      ( abs(((a).x-(b).x)) + abs((a).y-(b).y) )
#define VEC2_SQDIST(a,b)  ( ((a).x-(b).x) * (((a).x-(b).x)) + (((a).y-(b).y) * ((a).y-(b).y))  )
#define VEC2_CMP(a,b)   (! ( ((a).x - (b).y) | ((a).y - (b).y)) )
#define VEC2_DOT(a,b)   ( (a).y*(b).y + (a).x*(b).x )  

#define VEC2_CROSS(a,b)     VEC2( (a).y*(b).x, (a).x*(b).y )
#define VEC2_SUM(a,b)       VEC2( (a).y+(b).y, (a).x+(b).x )  
#define VEC2_SUB(a,b)       VEC2( (a).y-(b).y, (a).x-(b).x )  

// Vector - Vector (inplace)
#define VEC2_IDOT(a,b)       { (a).x*=(b).x; (a).y*=(b).y }
#define VEC2_ICROSS(a,b)     { (a).x*=(b).y; (a).y*=(b).x }
#define VEC2_ISUM(a,b)       { (a).x+=(b).x; (a).y+=(b).y }
#define VEC2_ISUB(a,b)       { (a).x-=(b).x; (a).y-=(b).y }

// Vector - Scalar
#define VEC2_ISMUL(a, s) { (a).x*=s; (a).y*=s }
#define VEC2_ISSUM(a, s) { (a).x+=s; (a).y+=s }
#define VEC2_ISSUB(a, s) { (a).x-=s; (a).y-=s }

// Vector 
#define VEC2_SQLEN(a) ( (a).x*(a).x + (a).y*(a).y )

// Point inside area
#define VEC2_INSIDE_RECT(p, ul, lr) ( (ul).y<=(p).y && (ul).x<=(p).x && (p).y <=(lr).y && (p).x<=(lr).x )

#endif /* _TYPES_H_ */