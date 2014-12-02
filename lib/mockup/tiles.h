#ifndef MOCKUP_TILES_HPP
#define MOCKUP_TILES_HPP

#include <stdint.h>

struct tile_flip {
    uint8_t flipX :1;
    uint8_t flipY :1;
};
typedef struct tile_flip tile_flip_t;

tile_flip_t tile_flip_None = { .flipX = 0, .flipY = 0};
tile_flip_t tile_flip_X = { .flipX = 1, .flipY = 0};
tile_flip_t tile_flip_Y = { .flipX = 0, .flipY = 1};
tile_flip_t tile_flip_XY = { .flipX = 1, .flipY = 1};

struct tile8 {
    uint8_t pixels[64];
};
typedef struct tile8 tile8_t;
    
struct tile16 {
    uint32_t pixels[256];
};
typedef struct tile16 tile16_t;


tile8_t tile8_flip(tile8_t tile, tile_flip_t flip);
tile8_t tile8_from3bpp(uint8_t* data);
tile8_t tile8_from4bpp(uint8_t* data);

tile16_t tile16_fromTile8(tile8_t t[4], uint8_t* palette);
    
#endif //MOCKUP_TILES_HPP