#ifndef _MOVEMENT_H_
#define _MOVEMENT_H_

#include "entity.h"

enum direction {
    D_U = 1,
    D_UR,
    D_R,
    D_DR,
    D_D,
    D_DL,
    D_L,
    D_UL,
};

/* Simple movement to coordinate*/
// int moveto( entity_t* e );

/* Clockwise*/
// int patrol_square( entity_t* e, coord_t target );

/* Clockwise */
/* 
    target - upper-left corner of square
    u.radius - square side's length
*/
int walk_around_square( entity_t* e );

#endif /* _MOVEMENT_H_ */
