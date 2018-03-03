#include "types.h"
#include "message.h"
#include "squaere.h"
#include "movement.h"

// int squaere_global_state( entity_t* e ){
//     (void) e;
//     return 0;
// }

int squaere_state_update( entity_t* e ){

    e->movtype->mov(e);
    // (void) e;
    return 0;
}

size_t squaere_memsize(){
    return 
        sizeof(entity_t) + 
        sizeof(state_t) + 
        sizeof(movtype_t);
}

// CTOR
entity_t* squaere_create( entity_t* ent, level_t* lvl, vec2_t pos ){

    void* mem = (void*) ent;

    if( NULL == mem ){

        mem = malloc(
                squaere_memsize()
            );

        if( NULL == mem ) return NULL;
    }


    entity_t* e = (entity_t*) mem;
    state_t* s = (state_t*) PTR_OFFSET(mem, sizeof(entity_t));
    movtype_t* m = (movtype_t*) PTR_OFFSET(s, sizeof(state_t));
    

    entity_t entity = {
        .pos      = pos,
        .level    = lvl,
        .type     = E_HUMAN,
        .traits   = TRAIT_BLIND,
        .gstate   = NULL,
        .state    = s,
        .movtype  = m,
    };

    state_t state = {
        .receive  = dummy_state_receive,
        .update   = squaere_state_update,
    };

    movtype_t movtype = {
        .target   = VEC2(0,0),
        .mov      = walk_around_square,
    };

    *e = entity;
    *s = state;
    *m = movtype;

    npc_create(e);

    return e;
}