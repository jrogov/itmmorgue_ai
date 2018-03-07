#ifndef _ENTITY_LIST_H_
#define _ENTITY_LIST_H_

#include "squaere.h"
#include "guard.h"


enum ENTITY_TYPE {
    ENT_DUMMY = 0,
    ENT_SQUAERE,
    ENT_GUARD 

};

#define ENTITY_CREATE(name, ent, lvl, pos) ctor[ENT_##name]((ent), (lvl), (pos));

ent_ctor_t ctor[] = {
    // dummy_create,
    NULL,
    squaere_create,
    guard_create
}; 

#endif /* _ENTITY_LIST_H_ */
