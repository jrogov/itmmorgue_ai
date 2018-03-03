/*
    
    Find NPC at position

    STRUCTURE

    Consists of 3 "nested" structures:

    Array of levels: ( level_t.chunkroot_t )
        (Possibly) Each level has it's own data for searching for npcs
    |
    |
    Array of chunks: (chunkroot_t.chunks[])
        One chunk describes part of map (x,y) - (x+S-1, y+S-1)
        S(ize) - length of chunk's side
    |
    |
    Double-Linked List of NPCs: (entity_t.entloc)
        List of NPCs at specific chunk
        ! Each NPC has it's own reusable DLL element initialized at start


    ARRAY OF CHUNKS

    -----------------
    | 0 | 1 | 2 | 3 |
    |---------------|
    | 4 | 5 | 6 | 7 |
    |---------------|
    | 8 | 9 | 10| 11|
    |---------------|
    | 12| 13| 14| 15|
    -----------------

    MOVEMENT
        If NPC moves inside chunk, nothing's changed
        Else NPC's DLL element is reassigned to new chunk


    NOTES: 
        Checks only for NPCs, not players!
        Since there aren't many players, they could be checked with simple iteration

        Chunk Size is to be chosen carefully:
            Bigger chunk size - lower memory usage - slower NPC check 
            Lesser chunk size - higher memory usage - faster NPC check

        One chunk may cover incomplete part of level: i.e. cover field beyond boundaries
    
        Given # - level cell, following chunks are acceptable

        --------    --------    --------    --------    --------
        |###   |    |####  |    |###   |    |     #|    |####  |
        |###   |    |####  |    |###   |    |     #|    |######|
        |      |    |####  |    |      |    |     #|    |##    |
        --------    --------    --------    --------    --------

*/

#ifndef _CHUNK_H_
#define _CHUNK_H_

#include <stdint.h>

#include "types.h"
#include "common.h"

// Used for deducing chunk_size if none given
// TODO: effectively should be deduced from number of entitys and size of level
#define CHUNK_NUM 4096

// Size of chunk (i.e. number of cells/tiles it covers in one dimension) 
// TODO change back to 64
#define CHUNK_SIZE 16
#define CHUNK_SIZE_OFFSET log2c((CHUNK_SIZE))

// Minimum size of level to actually use chunks (use 1 "global" chunk otherwise)
#define CHUNK_SIZE_THRESHOLD 64

typedef struct chunkroot chunkroot_t;
typedef struct chunk chunk_t;
typedef struct entloc entloc_t;


// Special values for init.chunks_[xy]
#define CHUNKS_NONE -1  // Don't use chunks at all (i.e. 1 stub chunk)
#define CHUNKS_AUTO 0   // Automatically calculate size of chunks based on level size

struct chunkroot {

    union {
        // number of chunks in X and Y axes
        struct {
            int16_t chunks_y;
            int16_t chunks_x;    
        } init;

        // parameters for calc of chunk index. Filled in chunk_init_level.
        struct {
            uint16_t x_cnum;
            uint16_t y_offset;
            uint16_t x_offset;
        } calc;
    } u;

    uint32_t chunks_num;

    // Pointer to array of chunks
    chunk_t *chunks;
};


struct entloc {
    // next MUST be first: used in dirty trick with (entloc_t) struct chunk
    entloc_t* next;
    entloc_t* prev;

    entid_t id;
};


#define ISEMPTY_CHUNK(a) ( NULL == (a.entloc) )

struct chunk {
    // TODO? change to entloc?
    // entloc_t* entloc;
    entloc_t entloc;
};

int chunk_init_levels(int level_num, ...);

int chunk_index(const chunkroot_t* cr, vec2_t pos);
int chunk_are_neighbours(const chunkroot_t* cr, vec2_t a, vec2_t b);


/*
{
    playerat()
    npcat()
}
*/


#endif /* _CHUNK_H_ */