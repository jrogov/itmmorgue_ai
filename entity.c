#include "entity.h"
#include "chunk.h"

#define NPCS_NUM 4096
entity_t* npcs[NPCS_NUM];
entid_t npc_num = 0;

entity_t* get_entity(entid_t id){
    return npcs[id];
}

/*
    Make a turn in this tick
*/
int entity_update(entity_t* e){
    int ret = 0;

    if( NULL != e->groutine )
        ret |= e->groutine->update(e);

    if( NULL != e->routine )
        ret |= e->routine->update(e);

    // if( ret & C_ENT_MOVED )


    return ret;
}

/*
    Call this in the end of entity initialization
*/
int npc_create(entity_t* e){

    e->entlist = (entlist_t){
        .next = NULL,
        .prev = NULL
    };

    e->EID = npc_num;
    npcs[npc_num] = e;
    ++npc_num;

    entity_place(e);

    return 0;
}

/*
    Calculate speed for this turn
*/
pos_t get_speed(entity_t* e){

    movtype_t* m    = e->movtype; 
    int base_speed = m->speed.base;
    int chance     = m->speed.chance;

    static unsigned int mask = BITMASK_LSB(
        unsigned int, 
        8*sizeof(m->speed.chance)
    );

    int threshold = rand() & mask;

    return base_speed + ( (chance>threshold) ? 1 : 0);
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


/*
    Searches for NPCs at given area.
    
    lvl     : level we are searching
    area    : description of area
    mask    : return entity that satisfy some condition
    buffer  : buffer to fill with Entity IDs
              buffer[0] : length of array

    entid_t* : [0]  - number of entities found
               [1:] - array of Entity ID's located in this area

    Returned value should be free()'d
*/
entid_t* npc_at_area(const level_t* lvl, const area_t* area, const emask_t* mask, entid_t* buffer ){

    /* TODO check mask*/
    (void) mask;

    // TODO? redo this shit
    entid_t* arr;
    size_t arr_len;
    size_t arr_idx;

    arr_idx = 1;
    if( NULL == buffer ){
        arr_len = 16;
        arr = malloc(arr_len * sizeof(entid_t));
        if( NULL == arr )
            panic("Error allocating entid_t[]!");
    }
    else {
        arr_len = buffer[0];
        arr = buffer;
    }

    const chunkroot_t* cr = &lvl->chunkroot;

    switch( area->type ){
        case A_RECT:
                    do {} while(0);

                    vec2_t ul = area->u.rect.ul;
                    vec2_t lr = area->u.rect.lr;
                    vec2_t ur = VEC2(ul.y, lr.x);
                    vec2_t ll = VEC2(lr.y, ul.x);

                    uint32_t chunk_vert_inc = cr->u.calc.x_cnum;
                    uint32_t chunk_fline_start = chunk_index(cr, ul);
                    uint32_t chunk_rect_width = chunk_index(cr, ur) - chunk_fline_start + 1;
                    uint32_t chunk_vert_bound = chunk_index(cr, ll) + chunk_vert_inc;

                    for(uint32_t s=chunk_fline_start; s< chunk_vert_bound; s+=chunk_vert_inc){

                        uint32_t chunk_idx = s;
                        for(uint32_t len=0; len<chunk_rect_width; ++len, ++chunk_idx){

                            entlist_t* el = &(cr->chunks[chunk_idx].entlist);
                            
                            for( el = el->next; NULL != el; el = el->next ) {
                                entid_t id = el->id;
                                vec2_t pos = npcs[id]->pos;
                                if( VEC2_INSIDE_RECT(pos, ul, lr) ){
                                    arr[arr_idx] = id;
                                    ++arr_idx;
                                    if( arr_idx == arr_len ){
                                        arr_len <<= 1;
                                        arr = realloc(arr, arr_len * sizeof(entid_t));
                                    }
                                }
                            }
                        }
                    }
                        arr[0] = arr_idx-1;
                    return arr;

                    break;

        // case A_MCIRCLE:
        // case A_CIRCLE:
    }



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