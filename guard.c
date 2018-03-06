#include "types.h"
#include "movement.h"
#include "entity.h"
#include "guard.h"

enum substates {
    S_GUARD = 0,
    S_FOLLOW
};

int guard_state_update( entity_t* e ){

    switch(e->state->s){
        case S_GUARD:
            (void) e;
            entid_t* ids = npc_at_area(e->level, (area_t*) e->state->data, NULL, NULL);

            for(int i=ENTID_ARRAY_START; i<ENTID_ARRAY_LEN(ids); ++i){
                if(ids[i] != e->EID){
                    e->movtype->data = get_entity(ids[i]);
                    e->state->s = S_FOLLOW;
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
        sizeof(state_t)+
        sizeof(state_t)+
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

    entity_t*  e    = (entity_t*)  mem;
    state_t*   gs   = (state_t*)   PTR_OFFSET(e, sizeof(entity_t));
    state_t*   s    = (state_t*)   PTR_OFFSET(gs, sizeof(state_t));
    movtype_t* m    = (movtype_t*) PTR_OFFSET(s, sizeof(state_t));
    area_t*    area = (area_t*)    PTR_OFFSET(m, sizeof(area_t));

    *e = (entity_t) {
        .pos = pos,
        .level = lvl,
        .type = E_HUMAN,
        .traits = TRAIT_NONE,
        .gstate = NULL,
        .state = s,
        .movtype = m
    };

    *gs = (state_t) {
        .receive = dummy_state_receive,
        .update = dummy_state_update
    };

    *s = (state_t) {
        .s = S_GUARD,
        .receive = dummy_state_receive,
        .update = guard_state_update,
        .data = area
    };

    *m = (movtype_t){
        .data = NULL
    };

    npc_create(e);
    return e;
}
