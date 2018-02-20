#include <unistd.h>
#include <curses.h>
#include <locale.h>
#include <stdlib.h>

// TODO remove
#include "common.h"

#include "types.h"
#include "entity.h"
#include "message.h"
#include "squaere.h"
#include "movement.h"

// int squaere_global_state( entity_t* e ){
//     (void) e;
//     return 0;
// }

int squaere_state_receive( state_t* s, message_t* m ){
    (void) s;
    (void) m;
    return 0;
}

int squaere_state_update( entity_t* e ){

    e->movtype->mov(e);
    // (void) e;
    return 0;
}

// CTOR
entity_t* squaere_create( void* (* malloc_f )(size_t) ){

    void* mem = malloc_f(
        sizeof(entity_t) + 
        sizeof(state_t) + 
        sizeof(movtype_t)
        );

    entity_t* e = (entity_t*) mem;
    state_t* s = (state_t*) PTR_OFFSET(mem, sizeof(entity_t));
    movtype_t* m = (movtype_t*) PTR_OFFSET(s, sizeof(state_t));

    // TODO try { .x } type of definition
    e-> coord.x       = 0;
    e-> coord.y       = 0;
    e-> type    = E_HUMAN;
    e-> traits  = TRAIT_BLIND; 
    e-> gstate  = NULL;
    e-> state   = s;
    e-> movtype = m;

    s-> receive = squaere_state_receive;
    s-> update  = squaere_state_update;

    m-> target.x = 0;
    m-> target.y = 0;
    m-> mov     = walk_around_square;

    // TODO remove: only testing
    m-> u.sq.radius = 4;
    m-> u.sq.direction = D_R;

    return e;
}


//-------------------------------MAIN-------------------------------------------

int __debug_print_entity(entity_t* e){
    DEBUG_PRINTF(0,0,
        "Type: %hu\n"
        "Traits: %hx\n"
        "Coordinates: %hu : %hu\n",
        e->type,
        e->traits,
        e->coord.x, e->coord.y
        );
    return 0;
}

int main(){

    WINDOW* mainwin;
    // WINDOW * map;

    setlocale(LC_ALL, "");
    /*  Initialize ncurses  */
    if ( (mainwin = initscr()) == NULL ) {
        fprintf(stderr, "Error initialising ncurses.\n");
        exit(EXIT_FAILURE);
    }   

    DEBUG_WINDOW = newwin(30, 30, 0, 0);

    entity_t* e = squaere_create(malloc);

    __debug_print_entity(e);
    while( wgetch(DEBUG_WINDOW) != '\n' ){
        entity_update(e);
        __debug_print_entity(e);
    }

    endwin();

}