#ifndef _TYPES_H_
#define _TYPES_H_

#include <stdint.h>

typedef uint32_t tick_t;
typedef uint16_t pos_t;
typedef uint16_t len_t;

typedef struct struct_entity entity_t;
typedef struct struct_state state_t;
typedef struct struct_movtype movtype_t;
typedef struct struct_coordinates coord_t;

struct struct_coordinates {
    pos_t x;
    pos_t y;
};

#endif /* _TYPES_H_ */