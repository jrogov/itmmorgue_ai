#include "entity_routines.h"

int dummy_routine_update( entity_t* e ){
    (void)e;
    return 0;
}

int dummy_routine_receive( routine_t* s, message_t* m){
    (void)s;
    (void)m;
    return 0;
}