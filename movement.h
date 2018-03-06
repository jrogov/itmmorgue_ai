#ifndef _MOVEMENT_H_
#define _MOVEMENT_H_

#include "types.h"
#include "entity.h"

enum direction {
    D_NONE = 0,
    D_U,
    D_UR,
    D_R, 
    D_DR,
    D_D, 
    D_DL,
    D_L, 
    D_UL,
    D_NUM
};

#define DIR_VEC(a) (_dir_vec[(a)])
/* Simple movement to coordinate*/
// int moveto( entity_t* e );


/* Clockwise */
/* 
    target - upper-left corner of square
    u.radius - square side's length
*/

vec2_t get_dir_vec(enum direction);

int moveto( entity_t* e );
int simple_follow( entity_t* e );
int simple_follow( entity_t* e );
int walk_around_square( entity_t* e );

#endif /* _MOVEMENT_H_ */
