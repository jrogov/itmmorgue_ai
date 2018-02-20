#ifndef _SQUAERE_H_
#define _SQUAERE_H_

entity_t* squaere_create( void* (* malloc_f )(size_t) );

#define SQUAERE_ID ENTITY_ID( 1 )
#define SQUAERE_CTOR ENTITY_CTOR(squaere_create)

#endif /* _SQUAERE_H_ */