#include "common.h"
#include "chunk.h"

#include "entity.h"
#include "levels.h"


/*
    Calculate chunk index for given 
    cr : chunkroot for a given level
     and position
*/
int chunk_index(const chunkroot_t* cr, vec2_t pos){

    uint16_t y_offset = cr->u.calc.y_offset;
    uint16_t x_offset = cr->u.calc.x_offset;
    uint16_t x_cnum = cr->u.calc.x_cnum;

    return ( pos.y >> y_offset ) * x_cnum + ( pos.x >> x_offset ); 
}

// TODO: write as preprocessor function?
/*
    Checks if two vectors are positioned in the same chunk
*/
int chunk_are_neighbours( const chunkroot_t* cr, vec2_t a, vec2_t b){
    return 0 == ( ( (a.y ^ b.y) >> cr->u.calc.y_offset ) | ( (a.x ^ b.x) >> cr->u.calc.x_offset ) );
}

int chunk_init_level(level_t* lvl){

    chunkroot_t root = lvl->chunkroot;

    uint16_t chunks_num[2]; // y, x
    uint16_t chunk_size[2];
    uint16_t* maxs = &(lvl->max_y);
    int16_t* init_array = &(root.u.init.chunks_y);

    // Two passes: for Y(0) and X(1)
    for( int i=0; i<2; ++i ){
        switch( init_array[i] ){
            case CHUNKS_NONE:
                chunks_num[i] = 1;
                break;

            case CHUNKS_AUTO:
                if( maxs[i] >= CHUNK_SIZE_THRESHOLD ){
                    chunk_size[i] = CHUNK_SIZE;
                    chunks_num[i] = maxs[i] >> CHUNK_SIZE_OFFSET;
                }
                else
                    chunks_num[i] = 1;
                break;

            default:
                chunks_num[i] = init_array[i];
        }
    }

    chunkroot_t r = {
        .u.calc.y_offset = log2c(chunk_size[0]),
        .u.calc.x_offset = log2c(chunk_size[1]),
        .u.calc.x_cnum = chunks_num[1],
        .chunks_num = chunks_num[0] * chunks_num[1]
    };

    if( NULL == (r.chunks =  (chunk_t*)malloc( sizeof(chunk_t) * r.chunks_num) ))
        panic("[C] Error allocating chunks!");

    lvl->chunkroot = r;

    return 0;
}

/*
    Init chunk structures

    level_num : number of level_t passed as varargs
    ...       : (level_t*) pointer level_t structure to build chunk for  

*/
int chunk_init_levels(int level_num, ...){
    va_list valist;

    // TODO? one malloc for all chunks

    va_start(valist, level_num);
    for( int i=0; i<level_num; ++i ){
        level_t* lvl = va_arg(valist, level_t*);
        // if( NULL == lvl ) break;
        chunk_init_level(lvl);
    }

    va_end(valist);

    return 0;

}