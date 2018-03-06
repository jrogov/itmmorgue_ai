#include <limits.h>
#include "types.h"
#include "movement.h"


static const vec2_t dir_vec[D_NUM] = {
    [D_NONE] = SVEC2( 0, 0),
    [D_U]    = SVEC2(-1, 0),
    [D_UR]   = SVEC2(-1, 1),
    [D_R]    = SVEC2( 0, 1),
    [D_DR]   = SVEC2( 1, 1),
    [D_D]    = SVEC2( 1, 0),
    [D_DL]   = SVEC2( 1,-1),
    [D_L]    = SVEC2( 0,-1),
    [D_UL]   = SVEC2(-1,-1),
};


int moveto( entity_t* e ){
    vec2_t from = e->pos;
    vec2_t to   = e->movtype->target;

    int min = INT_MAX;
    enum direction direction;

    for( int d=0; d< D_NUM; ++d ){
        vec2_t pos = VEC2_SUM(from, dir_vec[d]);
        int dist = VEC2_MDIST(pos, to);
        if( dist <= min && dist != 0 ){
            min = dist;
            direction = d;
        }
    }

    if(D_NONE != direction) entity_move(e, VEC2_SUM(from, dir_vec[direction]));

    return 0;
}

int simple_follow( entity_t* e ){

    e->movtype->target = ( (entity_t*) e->movtype->data )->pos;
    moveto( e );

    return 0;


}


int walk_around_square( entity_t* e ){

    movtype_t* m = e->movtype;

    vec2_t ul = m->target;         //upper-left corner
    len_t r = m->u.sq.radius;
    uint8_t* d = &(m->u.sq.direction);

    #define X e->pos.x
    #define Y e->pos.y

    switch( m->u.sq.direction ){
        case D_R:
            entity_move(e, VEC2(Y, X+1));        
            if( X+1 >= ul.x+r)
                *d = D_D;
            break;

        case D_D:
            entity_move(e, VEC2(Y+1, X));
            if( Y+1 >= ul.y+r )
                *d = D_L;
            break;

        case D_L:
            entity_move(e, VEC2(Y, X-1));
            if( X-1 < ul.x )
                *d = D_U;
            break;

        case D_U:
            entity_move(e, VEC2(Y-1, X));
            if( Y-1 < ul.y )
                *d = D_R;
            break;
    }

    #undef X
    #undef Y

    return 0;
    
}