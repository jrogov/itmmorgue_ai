#include <unistd.h>
#include <curses.h>
#include <locale.h>
#include <stdlib.h>
#include <string.h>

#include "types.h"
#include "common.h"
#include "entity.h"
#include "movement.h"
#include "entity_list.h"
#include "chunk.h"

char glyphs[] = {
    [S_NONE]        = ' '   ,
    [S_FLOOR]       = '.'   ,
    [S_WALL]        = '#'   ,
    [S_DOOR]        = '|'   ,
    [S_TREE]        = '^'   ,
    [S_GRASS]       = ','   ,
    [S_PLAYER]      = '@'   ,
    [S_DOWNSTAIRS]  = '>'   ,
    [S_UPSTAIRS]    = '<'   ,

    [S_TRAP]        = '?'   ,
    [S_FOOD]        = '?'   ,

    [S_GOLD]        = '?'   ,
    [S_SCROLL]      = '?'   ,
    [S_BOOK]        = '?'   ,

    [S_RING]        = '?'   ,
    [S_WAND]        = '?'   ,
    [S_SIZE]        = '?'   
};

size_t lvltilepos(level_t* lvl, uint16_t y, uint16_t x) {
    return y * lvl->max_x + x;
}

#ifdef DEBUG
int __debug_print_entity(int16_t y, int16_t x, entity_t* e){

    if( y > 0 && x > 0 )
        wmove(DEBUG_WINDOW, y, x);

    DEBUG_PRINTF(
        "ID: %d"
            "\tType: %hu"
            "\tTraits: %hx\n"
        "Coordinates: %hu : %hu\n"
        "Chunk index: %d\n\n"
        ,
        e->entlist.id,
        e->type,
        e->traits,
        e->pos.y, e->pos.x
        ,chunk_index(&(e->level->chunkroot), e->pos)
        );
    return 0;
}

int __debug_print_chunk(int16_t y, int16_t x, level_t* lvl, int index){

    if( y > 0 && x > 0 )
        wmove(DEBUG_WINDOW, y, x);

    DEBUG_PRINTF("%03d | ", index);
    entlist_t* e = lvl->chunkroot.chunks[index].entlist.next;
    while( NULL != e ){
        DEBUG_PRINTF("%d -> ", e->id);
        e = e->next;
    }
        
    DEBUG_PRINT("nil\n");

    return 0;

}
#else
int __debug_print_entity(int16_t y, int16_t x, entity_t* e){ (void)y; (void)x; (void)e; return 0; }
int __debug_print_chunk(int16_t y, int16_t x, level_t* lvl, int index){ (void)y; (void)x; (void)lvl; (void)index; return 0; }
#endif

level_t* gen_simple_plane(vec2_t size, char* name){

    static int id = 0;

    uint32_t lvlsize = size.x * size.y;
    level_t* lvl = (level_t*) malloc(
        sizeof(level_t) + 
        sizeof(tile_t) * lvlsize
    );
    tile_t* tiles =  (tile_t*) (lvl+1);
    
    level_t lvl_temp = { 
        .id = id++,
        .max_y = size.y,
        .max_x = size.x,
        .size = lvlsize,
        .area = tiles,
        //.name
        .chunkroot.u.init = {
            .chunks_y = 0,
            .chunks_x = 0
        }
    };

    *lvl = lvl_temp;
    strncpy(lvl->name, name, MAX_LEVEL_NAME);

    tile_t buftile = {
        .top = S_FLOOR,
        .color = D_WHITE,
        .underlying = NULL
    };

    for( uint32_t y = 0; y < size.y; ++y )
        for( uint32_t x = 0; x < size.x; ++x ){
            buftile.y = y;
            buftile.x = x;
            tiles[y*size.x + x] = buftile;
        }

    return lvl;
}

int main(){

    WINDOW* mainwin;
    WINDOW * map;

    int xlen = 2048;
    int ylen = 2048;
    vec2_t winsize = { .y=50, .x=50 };
    vec2_t maxdraw = { .x=50, .y=50 };

    setlocale(LC_ALL, "");
    /*  Initialize ncurses  */
    if ( (mainwin = initscr()) == NULL ) {
        fprintf(stderr, "Error initialising ncurses.\n");
        exit(EXIT_FAILURE);
    }   

    map = newwin( winsize.y, winsize.x, 0, 0 );
    DEBUG_INIT(100, 50, 0, 52);
    cbreak();
    noecho();
    start_color();
    // raw();
    set_escdelay(0);
    // keypad(map, TRUE);
    curs_set(0);
    


    level_t* lvl = gen_simple_plane(VEC2(ylen, xlen), "Foo");
    chunk_init_levels(1, lvl);


    int entities_num = 10;
    entity_t* entities[entities_num];
    int ent_count = 0;
    vec2_t startpos = VEC2(1,1);

    entity_t* e = NULL;
    for( int i = 0; i<entities_num; ++i){
        vec2_t v = startpos;
        v = VEC2_SUM(v, VEC2(2*i,2*i));

        e = ctor[ENT_SQUAERE](NULL, lvl, v);
        e->movtype->target = v;
        e->movtype->u.sq.radius = 4*(entities_num-i);
        e->movtype->u.sq.direction = D_R;
        entities[ent_count++] = e;
    }

    for( uint32_t i=0; i < lvl->size; ++i){
        tile_t t = lvl->area[i];
        mvwaddch(map, t.y, t.x, glyphs[t.top]);
    }

    for( uint32_t y=0; y < maxdraw.y; ++y){
        for( uint32_t x=0; x < maxdraw.x; ++x){
            tile_t t = lvl->area[lvltilepos(lvl, y, x)];
            mvwaddch(map, t.y, t.x, glyphs[t.top]);
        }
    }

    for( int i=0; i<ent_count; ++i){
        vec2_t p = entities[i]->pos;
        mvwaddch(map, p.y, p.x, glyphs[S_PLAYER]);
    }

    wrefresh(map);
    
    DEBUG_MPRINTF(2,0,
            "Ceiled Log of Y Chunk Size: %d\n"
            "Ceiled Log of X Chunk Size: %d\n"
            "X Chunks Num: %d\n",
            lvl->chunkroot.u.calc.y_offset,
            lvl->chunkroot.u.calc.x_offset,
            lvl->chunkroot.u.calc.x_cnum
        );
    DEBUG_PRINT("\n  Y   X | EntID\n");
    DEBUG_FIXPOS();

    for(;;){

        //------------------------------------------------------------------------------
        //  DEBUG INFO
        //------------------------------------------------------------------------------

        DEBUG_RESET();

        for( int i = 0; i<entities_num; ++i)
        {
            vec2_t v = startpos;
            v = VEC2_SUM(v, VEC2(i,i));
            entid_t id = npc_at(lvl, v, NO_EMASK);
            DEBUG_PRINTF("%3d %3d | %3d\n", 
                v.y, v.x,  id
                );
        }
        DEBUG_PRINT("\n");

        // wmove(DEBUG_WINDOW, 5,0);
        for( int i = 0; i<ent_count; ++i)
            __debug_print_entity(-1,-1,entities[i]);

        uint16_t chunk_idxs[] = {0, 1, 2, 128, 129, 130, 256, 257, 258};

        for( size_t i =0; i<ARRAY_LEN(chunk_idxs); ++i)
        __debug_print_chunk(-1,-1,lvl,chunk_idxs[i]);

        DEBUG_REFRESH();

        //------------------------------------------------------------------------------
        

        #define CASE_INS(a) ((a) & ~0x0020)
        if( CASE_INS(wgetch(map)) == 'Q'  )
            break;
        #undef CASE_INS


        for( int i=0; i<ent_count; ++i){
            entity_t* e = entities[i];
            vec2_t p = e->pos;
            tile_t t = lvl->area[lvltilepos(lvl, p.y, p.x)];
            mvwaddch(map, t.y, t.x, glyphs[t.top]);

            entity_update(e);
            mvwaddch(map, e->pos.y, e->pos.x, glyphs[S_PLAYER]);
        }

        wrefresh(map);


    }

    endwin();

}