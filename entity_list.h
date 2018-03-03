#ifndef _ENTITY_LIST_H_
#define _ENTITY_LIST_H_

#include "squaere.h"
// #include "guard.h"


enum ENTITY_ID {
    ENT_DUMMY = 0,
    ENT_SQUAERE = 1,
    // ENT_GUARD
};

ent_ctor_t ctor[] = {
    // dummy_create,
    NULL,
    squaere_create,
    // guard_create
}; 

#endif /* _ENTITY_LIST_H_ */
