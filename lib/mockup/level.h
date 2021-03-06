#ifndef MOCKUP_LEVEL_H
#define MOCKUP_LEVEL_H

#include "r65816/r65816.h"
#include "gfx_store.h"
#include "layer.h"
#include "overworld.h"
#include "palette.h"
#include "tiles.h"
#include "tileset.h"

#define SPRITES

typedef struct {
    u16 x;
    u16 y;
    u16 tile;
    u16 is_top;
} object_tile_t;

typedef struct {
    u16 x;
    u16 y;
    u8 num;
    u8 settings;

    u16  bb_xmin;
    u16  bb_ymin;
    u16  bb_xmax;
    u16  bb_ymax;
    u16  zindex;
    u16* tiles;
} object_pc_t;

typedef struct {
    object_pc_t* objects;
    u16 length;
} object_list_pc_t;


#ifdef SPRITES
typedef struct {
    u8 y1         : 4;
    u8 extra_bits : 2;
    u8 screen1    : 1;
    u8 y2         : 1;
    u8 x          : 4;
    u8 screen2    : 4;
    u8 number;
} sprite_t;

typedef struct {
    int x;
    int y;
    u8 flip_x : 1;
    u8 flip_y : 1;
    
    u8 palette;
    u16 tile_num;
} sprite_tile_t;

typedef struct {
    u8 entry_009E;
    u8 entry_00AA;
    u8 entry_00B6;
    u8 entry_00C2;
    u8 entry_00D8;
    u8 entry_00E4;
    u8 entry_14C8;
    u8 entry_14D4;
    u8 entry_14E0;
    u8 entry_14EC;
    u8 entry_14F8;
    u8 entry_1504;
    u8 entry_1510;
    u8 entry_151C;
    u8 entry_1528;
    u8 entry_1534;
    u8 entry_1540;
    u8 entry_154C;
    u8 entry_1558;
    u8 entry_1564;
    u8 entry_1570;
    u8 entry_157C;
    u8 entry_1588;
    u8 entry_1594;
    u8 entry_15A0;
    u8 entry_15AC;
    u8 entry_15B8;
    u8 entry_15C4;
    u8 entry_15D0;
    u8 entry_15DC;
    u8 entry_15EA;
    u8 entry_15F6;
    u8 entry_1602;
    u8 entry_160E;
    u8 entry_161A;
    u8 entry_1626;
    u8 entry_1632;
    u8 entry_163E;
    u8 entry_164A;
    u8 entry_1656;
    u8 entry_1662;
    u8 entry_166E;
    u8 entry_167A;
    u8 entry_1686;
    u8 entry_186C;
    u8 entry_187B;
    u8 entry_190F;
    u8 entry_1FE2;
} sprite_table_entries_t;

typedef struct {
    int x, y;
    u8 number;
    u8 extra_bits;
    int num_tiles;
    sprite_tile_t* tiles;
    sprite_table_entries_t table_entries;
} sprite_pc_t;

typedef struct {
    int length;
    sprite_pc_t* data;   
} sprite_list_pc_t;
#endif

typedef struct {
    u8* layer1_objects_data;
    int layer1_objects_length;
} level_save_locations_t;


typedef struct {
    u8 screens         : 5;
    u8 bg_palette      : 3;

    u8 level_mode      : 5;
    u8 bg_color        : 3;
    
    u8 sprite_set      : 4;
    u8 music           : 3;
    u8 layer3_priority : 1;

    u8 fg_palette      : 3;
    u8 sprite_palette  : 3;
    u8 time            : 2;
    
    u8 tile_set        : 4;
    u8 vert_scroll     : 2;
    u8 item_memory     : 2;
} level_header_t;

typedef struct {
    level_header_t header;
    object_list_pc_t layer1_objects;
#ifdef SPRITES
    sprite_list_pc_t sprites;
    tileset_t sprite_tileset;
    map8_t    sprite_map8;
#endif
    union {
        object_list_pc_t layer2_objects;
        layer16_t        layer2_background;
    };
    u8 has_layer2_bg      : 1;
    u8 has_layer2_objects : 1;
    u8 is_vertical_level  : 1;
    u8 is_boss_level      : 1;

    u32  background_color;
    
    palette_t palette;
    tileset_t tileset;
    map8_t    map8;
    map16_t   map16_fg;
    map16_t   map16_bg;
    int       num_level;
    int       height;
    int       width;


    r65816_rom_t* rom;

} level_pc_t;


void level_init(level_pc_t* l, r65816_rom_t* rom, int num_level, gfx_store_t* gfx_store);
void level_init_by_address(level_pc_t* l, r65816_rom_t* rom, int num_level, gfx_store_t* gfx_store,
                           u32 level_layer1_data_addr_sfc,
                           u32 level_layer2_data_addr_sfc,
                           u32 level_sprite_data_addr_sfc);

void level_deinit(level_pc_t* l);

int level_object_list_pc_to_level_data(object_list_pc_t* object_list, u8* output_data, int max_length);
void level_move_object(level_pc_t* l, int index, u16 x, u16 y, gfx_store_t* gfx_store);

void level_animate(level_pc_t* l, u8 frame, gfx_store_t* gfx_store);


#endif //MOCKUP_LEVEL
