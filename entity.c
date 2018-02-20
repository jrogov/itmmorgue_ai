#include <stdlib.h>

#include "entity.h"

int entity_update(entity_t* e){
    int ret = 0;

    if( NULL != e->gstate )
        ret |= e->gstate->update(e);

    if( NULL != e->state )
        ret |= e->state->update(e);

    return ret;
}