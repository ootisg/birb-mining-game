#ifndef TILE_MAP_H
#define TILE_MAP_H

#include "game_object.h"

#define MAP_TILE_SIZE .0625f
#define MAP_GRID_WIDTH 100
#define MAP_GRID_HEIGHT 100

#define TILE_SKY_1 0

#define TILE_BG_1 1
#define TILE_BG_2 9

#define TILE_DIRT_1 2
#define TILE_DIRT_2 10

#define TILE_GRASS_1 3

#define TILE_COAL_1 4

struct map_tile {
	int x;
	int y;
	int id;
	rectangle bounds;
	game_object* tile_obj;
};

typedef struct map_tile map_tile;

void make_map_tile (void* ptr);

void init_map_tiles ();
void generate_tiles ();
map_tile* get_tile_grid ();
map_tile* map_get_tile (int x, int y);
sprite* get_tileset ();

void update_tile_objs ();

#endif