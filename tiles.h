// vim: sw=4 ts=4 et :
#ifndef TILES_H
#define TILES_H

enum colors {
    D_WHITE, D_YELLOW, D_RED, D_GREEN, D_BLUE, D_BLACK, D_CYAN, D_MAGENTA,
    L_WHITE, L_YELLOW, L_RED, L_GREEN, L_BLUE, L_BLACK, L_CYAN, L_MAGENTA
};

enum stuff {
    S_NONE            ,
    S_FLOOR           ,
    S_WALL            ,
    S_DOOR            ,
    S_TREE            ,
    S_GRASS           ,
    S_PLAYER          ,
    S_DOWNSTAIRS      ,
    S_UPSTAIRS        ,

    S_TRAP            ,
    S_FOOD            ,

    S_GOLD            ,
    S_SCROLL          ,
    S_BOOK            ,

    S_RING            ,
    S_WAND            ,
    S_SIZE
};


typedef struct tile {
    enum stuff top;             // type of the tile
    enum colors color;          // server-specified attributes
    uint16_t y;                 // absolute Y
    uint16_t x;                 // absolute X
    struct tile *underlying;    // pointer to underlying tile
} tile_t;

typedef struct tileblock {
    uint32_t count;
    uint32_t zcount;
    tile_t tiles[1];
} tileblock_t;

#endif /* TILES_H */
