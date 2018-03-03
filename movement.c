#include "types.h"
#include "movement.h"


int moveto( entity_t* e ){
    (void) e;
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