#include "entity.h"
#include "chunk.h"

#define NPCS_NUM 4096
entity_t* npcs[NPCS_NUM];
entid_t npc_num = 0;

int dummy_state_update( state_t* s ){
    (void)s;
    return 0;
}

int dummy_state_receive( state_t* s, message_t* m){
    (void)s;
    (void)m;
    return 0;
}

int entity_update(entity_t* e){
    int ret = 0;

    if( NULL != e->gstate )
        ret |= e->gstate->update(e);

    if( NULL != e->state )
        ret |= e->state->update(e);

    // if( ret & C_ENT_MOVED )


    return ret;
}

int npc_create(entity_t* e){

    e->entloc.id = npc_num;
    npcs[npc_num] = e;
    ++npc_num;

    entity_place(e);

    return 0;
}

/*
    chunk-related functions
*/


/*
    Find entity at position if there's any

    lvl  : level we are searching
    pos  : position at which we want to find entity
    mask : return entity that satisfy some condition
*/

// TODO: return multiple entities

entid_t npc_at(const level_t* lvl, vec2_t pos, const emask_t* mask){

    const chunkroot_t* cr = &(lvl->chunkroot);
    chunk_t* chunks = cr->chunks;

    entlist_t* el = &(chunks[chunk_index(cr, pos)].entlist);

    /* TODO: check mask */
    (void) mask;

    while( el != NULL ){
        entid_t id = el->id;
        if( VEC2_CMP(npcs[id]->pos, pos) )
            return id;

        el = el->next;
    }

    return -1;
}

entid_t player_at(const level_t* lvl, vec2_t pos, const emask_t* mask){
    // TODO
    (void) lvl;
    (void) pos;
    (void) mask;
    return 0;
}

entid_t entity_at(level_t* lvl, vec2_t pos, emask_t mask){
    // TODO
    (void) lvl;
    (void) pos;
    (void) mask;
    return 0;
}

int entity_place(entity_t* e){

    chunkroot_t* cr = &(e->level->chunkroot);
    chunk_t* c = &(cr->chunks[chunk_index(cr, e->pos)]);

    if( NULL != c->entlist.next )
        c->entlist.next->prev = &(e->entlist); 
    e->entlist.next = c->entlist.next;
    e->entlist.prev = &(c->entlist);
    c->entlist.next = &(e->entlist);

    return 0;
}

int entity_move(entity_t* e, vec2_t newpos){

    // TODO

    vec2_t pos = e->pos;
    chunkroot_t* cr = &(e->level->chunkroot);

    // TODO?: check level boundaries?

    e->pos = newpos;
    if( chunk_are_neighbours(cr, pos, newpos) )
        return 0;
    
    entlist_t* el = &(e->entlist);
    el->prev->next = el->next;
    if( NULL != el->next )
        el->next->prev = el->prev;

    return entity_place(e);

}