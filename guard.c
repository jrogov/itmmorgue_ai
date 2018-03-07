#include "guard.h"

#include "types.h"
#include "entity_routines.h"
#include "movement.h"

enum states {
    S_GUARD = 0,
    S_FOLLOW
};

struct guard_data {
    uint16_t guard_pos;

    area_t guard_area;
};

int guard_routine_update( entity_t* e ){

    switch(e->routine->s){
        case S_GUARD:
            (void) e;
            entid_t* ids = npc_at_area(e->level, (area_t*) e->routine->data, NULL, NULL);

            for(int i=ENTID_ARRAY_START; i<ENTID_ARRAY_LEN(ids); ++i){
                if(ids[i] != e->EID){
                    e->movtype->data = get_entity(ids[i]);
                    e->routine->s = S_FOLLOW;
                    return 0;
                }
            }
            // free(ids);

            break;

        case S_FOLLOW:
            (void) e;
            simple_follow(e);
            break;

    }

    return 0;
}

size_t guard_memsize(){
    return 
        sizeof(entity_t)+
        sizeof(routine_t)+
        sizeof(routine_t)+
        sizeof(movtype_t)+
        sizeof(area_t);
}

entity_t* guard_create( entity_t* ent, level_t* lvl, vec2_t pos){

    void* mem = (void*) ent;
    if( NULL == mem ){
        mem = malloc(
                guard_memsize()
            );

        if( NULL == mem ) return NULL;
    }

    // TODO CHANGE TO NEW FORMAT

    entity_t*   e    =  (entity_t*)  mem;
    routine_t*  gs   =  (routine_t*) PTR_OFFSET(e,  sizeof(entity_t));
    routine_t*  s    =  (routine_t*) PTR_OFFSET(gs, sizeof(routine_t));
    movtype_t*  m    =  (movtype_t*) PTR_OFFSET(s,  sizeof(routine_t));
    area_t*     area =  (area_t*)    PTR_OFFSET(m,  sizeof(area_t));

    *e = (entity_t) {
        .pos = pos,
        .level = lvl,
        .type = E_HUMAN,
        .traits = TRAIT_NONE,
        .groutine = NULL,
        .routine = s,
        .movtype = m
    };

    *gs = (routine_t) {
        .receive = dummy_routine_receive,
        .update = dummy_routine_update
    };

    *s = (routine_t) {
        .s = S_GUARD,
        .receive = dummy_routine_receive,
        .update = guard_routine_update,
        .data = area
    };

    *m = (movtype_t){
        .data = NULL
    };

    npc_create(e);
    return e;
}
