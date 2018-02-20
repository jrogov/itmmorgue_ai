#ifndef _MESSAGE_H_
#define _MESSAGE_H_

#include "event.h"

typedef struct struct_message message_t;

enum message_type {
    MSG_PERSONAL = 1,  // Message for specific entity

    // TODO
    // MSG_LOCATION,      // Message for entities on specific location 
    //                    // TODO: Separate structure to describe affected location
    // MSG_LEVEL,         // Message for all entities on specific level
    // MSG_LEVEL_MASK,    // Message for levels that meet type () 

    MSG_GLOBAL,        // Message to all entities

    // These can be combined with any other type above
    // i.e. these are mere specificators
    // MSG_ENT_TYPE = 16,    // Message for entities of specific type
    // MSG_ENT_MASK = 32,    // Message for entities with specific traits

    MSG_DELAYED = 64

};

struct struct_message {

    short message_type;


    // union {

        tick_t delay;


    // }

    void* extra;
    
};

#endif /* _MESSAGE_H_ */
