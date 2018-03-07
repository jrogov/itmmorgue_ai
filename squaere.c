#include "squaere.h"

#include "types.h"
#include "entity_routines.h"
#include "movement.h"
// #include "message.h"

// int squaere_global_routine( entity_t* e ){
//     (void) e;
//     return 0;
// }

#define ENT squaere

// int squaere_routine_update( entity_t* e )

ROUTINE_DEF(WALK)
{
    e->movtype->mov(e);
    // (void) e;
    return 0;
}

size_t squaere_memsize(){
    return 
        sizeof(entity_t) + 
        sizeof(routine_t) + 
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
    routine_t* s = (routine_t*) PTR_OFFSET(mem, sizeof(entity_t));
    movtype_t* m = (movtype_t*) PTR_OFFSET(s, sizeof(routine_t));
    

    entity_t entity = {
        .pos      = pos,
        .level    = lvl,
        .type     = E_HUMAN,
        .traits   = TRAIT_BLIND,
        .groutine   = NULL,
        .routine    = s,
        .movtype  = m,
    };

    routine_t routine = {
        .receive  = dummy_routine_receive,
        .update   = ROUTINE(WALK),
    };

    movtype_t movtype = {
        .target   = VEC2(0,0),
        .mov      = walk_around_square,
    };

    *e = entity;
    *s = routine;
    *m = movtype;

    npc_create(e);

    return e;
}

#undef ENT