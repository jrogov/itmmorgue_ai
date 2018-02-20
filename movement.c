#include "types.h"
#include "movement.h"

int walk_around_square( entity_t* e ){


    movtype_t* m = e->movtype;

    coord_t ul = m->target;         //upper-left corner
    len_t r = m->u.sq.radius;
    uint8_t* d = &(m->u.sq.direction);

    #define X e->coord.x
    #define Y e->coord.y

    switch( m->u.sq.direction ){
        case D_R:
            X++;        
            if( X+1 >= ul.x+r)
                *d = D_D;
            break;

        case D_D:
            Y++;
            if( Y+1 >= ul.y+r )
                *d = D_L;
            break;

        case D_L:
            X--;
            if( X-1 < ul.x )
                *d = D_U;
            break;

        case D_U:
            Y--;
            if( Y-1 < ul.y )
                *d = D_R;
            break;
    }

    return 0;
    
}
