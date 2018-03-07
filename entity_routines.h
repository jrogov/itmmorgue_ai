#ifndef _ENTITY_ROUTINES_H_
#define _ENTITY_ROUTINES_H_

#include "entity.h"
#include "message.h"

/*
    Some routines that may be shared by more than one type of entity
*/

// Dummy routine functions: does nothing;
int dummy_routine_update( entity_t* e );
int dummy_routine_receive( routine_t* s, message_t* m);


#endif /* _ENTITY_ROUTINES_H_ */