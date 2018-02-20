/*
    TODO
    move movement definitions (movtype_t etc.) to separate header (movement.h)
    
*/

#ifndef _ENTITY_H_
#define _ENTITY_H_

#include <stdint.h>

#include "types.h"

#include "message.h"
#include "movement.h"

#define ENTITY_ID(a) (a)
#define ENTITY_CTOR(a) (a)      // Function for creating entity
                                // entity_t* ctor( void* (* malloc_f )(size_t) 

#define MAX_WEIGHT UINT16_MAX

/*
    AI 
        Any entity consists of one or more states:
        - global state (opt.) - describes behaviour and states change
        - current state - describes current "task" of entity
        - previous states (opt.) - prev. states with priorities 

    State
        Simple FSM that describes entity behaviour
        State may change only on the end of turn

    Weights
        If state changes, it changes to state with maximum weight
        Weight is being caluclated with each state_t.receive call,
        i.e. each message either affects 1 or more weights, or it is ignored

    (TODO?: Low-health is checked inside state_t.update OR received as message )
    (TODO?: state-change only if weight exceeds threshold )
*/


// state_t
struct struct_state {

    int (* receive )(state_t*, message_t*);    // Receives messages and updates state weights
                                     // NULL for simple mobs

    int (* update )(entity_t*);      // State logic

    void* data;                      // Extra data for states (i.e. buffer)
                                     // TODO? Managed by state itself 

    // int state_num;
    // uint16_t weights[/*state_num*/];
};



// Entity type: similar to race, but more broad
enum entity_type {
    E_UNDEF = 0,

    E_HUMAN = 1,
    // E_DWARF,
    // E_ELF
    // etc.    

    E_OBSTACLE,  // Some obstacle one can interact with. 
                 // Can be anything from 1x1 wall to building

    E_TRAP,      // This shit may kill you.


};


// Traits: things that describe entity
// CAN be combined with bitwise OR
enum entity_traits {
    TRAIT_NONE = 0,

    TRAIT_CONSCIOUS = 0x0001,
    TRAIT_BLIND     = 0x0002,
    // TRAIT_USESMAGIC = 0x0004
};


// entity_t
struct struct_entity {

    coord_t coord;
    // pos_t x;
    // pos_t y;

    // uint16_t entid;                // Entity ID, i.e. 
    uint16_t type;                 // TODO: do we need this?
    // TODO: Increase size?
    uint16_t traits;

    // level_t* level;                // Level on which entity currently located

    state_t* gstate;               // Global state - defines state management and basic behaviour
    state_t* state;                // Current state - defines current task (patrol, pursue, attack, etc.)

    // pqueue<state_t> prev_states // Previous states - states entity may return to.
                                   // Each state has it's own (dynamic) priority
                                   // State with max priority is restored as current
                                   // Common 
                                   // Capacity depends from entity params (e.g. state_mem_capacity (TODO) )


    // Movement
    // TODO: REDO TO struct 
    movtype_t* movtype;
    // int (* move )(coord_t*); 
    

    // (coordinates_t or entity_t?) target





};

// Movement type
// search: Steering Behaviour
struct struct_movtype {

    coord_t target;
    union {
        struct {
            len_t radius;
            uint8_t direction;
        } sq;
    } u;
    int (* mov )(entity_t*); 

    // void* data;

};


int entity_update( entity_t* );

#endif /* _ENTITY_H_ */