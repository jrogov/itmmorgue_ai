/*
    TODO
    move movement definitions (movtype_t etc.) to separate header (movement.h)
    
*/

#ifndef _ENTITY_H_
#define _ENTITY_H_

#include <stdint.h>

#include "types.h"
#include "chunk.h"
#include "levels.h"
#include "message.h"

#define ENTITY_ID(a) (a)
#define ENTITY_CTOR(a) (a)      // Function for creating entity
                                // entity_t* ctor( void* (* malloc_f )(size_t) 

// TODO switch state macro
#define SWITCH(e,name) e->state = 

#define GLOBAL_STATE    ENTNAME##_global_state
#define STATE(name)     ENTNAME##name##_state_update

#define GLOBAL_STATE_DEF    int GLOBAL_STATE( entity_t* e )
#define STATE_DEF(name)     int STATE(name)( entity_t* e )


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

// Entity Related Types

typedef struct entity entity_t;
typedef struct state state_t;
typedef struct movtype movtype_t;

// ent_ctor_t - constructor for some entity
// If parameter is not NULL, used for new entity
// Allocated with malloc() otherwise
// typedef void* (*malloc_t) (size_t);
typedef entity_t* (*ent_ctor_t)( entity_t*, level_t*, vec2_t );


// Entity Masks Types

typedef struct emask_predicate emask_predicate_t;
typedef struct emask emask_t;


// state_t
struct state {

    int s;                                  // Current substate in FSM: defined in entity module

    int (* receive )(state_t*, message_t*); // Receives messages and updates state weights
                                            // NULL for simple mobs

    int (* update )(entity_t*);             // State logic

    void* data;                             // Extra data for states (i.e. buffer)
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

    TRAIT_CONSCIOUS   = 0x0001,
    TRAIT_BLIND       = 0x0002,
    TRAIT_INCORPOREAL = 0x0004, // can be walked through
    
    // TRAIT_USESMAGIC
};


// entity_t
#define EID entlist.id
struct entity {

    vec2_t pos;
    level_t* level;

    uint16_t type;                 // TODO: do we need this?
    // TODO: Increase size?
    uint16_t traits;

    struct {
        uint8_t base;
        uint8_t chance;           // 
    } speed;

    state_t* gstate;               // Global state - defines state management and basic behaviour
    state_t* state;                // Current state - defines current task (patrol, pursue, attack, etc.)

    // pqueue<state_t> prev_states // Previous states - states entity may return to.
                                   // Each state has it's own (dynamic) priority
                                   // State with max priority is restored as current
                                   // Common 
                                   // Capacity depends from entity params (e.g. state_mem_capacity (TODO) )


    movtype_t* movtype;

    entlist_t entlist;

    // fighttype_t* fighttype;

};

// Movement type
// search: Steering Behaviour
struct movtype {

    vec2_t target;

    union {
        struct {
            len_t radius;
            uint8_t direction;
        } sq;
    } u;

    // vec2_t** targets;
    int (* mov )(entity_t*); 
    void* data;

};


// ENTITY MASKS: Select entities that satisfy masks

#define NO_EMASK NULL
#define NO_PREDICATE NULL

struct emask_predicate {

    /* TODO */

    emask_predicate_t* next;
};

enum emask_type {
    MASK_NONE     = 0,
    // MASK_PERSONAL = 1,
};

struct emask {
    uint16_t type;                // Mask type                   
    uint16_t traits;              // Traits that entity must have
                                                           
    emask_predicate_t* predicate; // Complex masks     
};



//------------------------------------------------------------------------------
//    FUNCTIONS
//------------------------------------------------------------------------------

enum update_code {
    C_ENT_MOVED   = 0x0001,
    C_ENT_DAMAGED = 0x0002,

    // C_ENT_DIED = 
};

// "Getters" that has complex calculation
entity_t* get_entity(entid_t id);
pos_t get_speed(entity_t* e);

// Dummy state functions: does nothing;
int dummy_state_update( entity_t* e );
int dummy_state_receive( state_t* s, message_t* m);

int npc_create( entity_t* );
int entity_update( entity_t* );


// Chunk-related functions

// Place entity (in chunk)  
int entity_place( entity_t* );
// To reposition entity (correctly for chunks)
int entity_move( entity_t*, vec2_t );


// Find entity at given position, be it NPC or Player
entid_t entity_at(level_t* lvl, vec2_t pos, emask_t mask);

// Find NPC at given position 
entid_t npc_at(const level_t* lvl, vec2_t pos, const emask_t* mask);
// Find player at given position
entid_t player_at(const level_t* lvl, vec2_t pos, const emask_t* mask);

#define ENTID_ARRAY_LEN(buf)   ((buf)[0])
#define ENTID_ARRAY_START      1
entid_t* npc_at_area(const level_t* lvl, const area_t* area, const emask_t* mask, entid_t* arr);


#endif /* _ENTITY_H_  */