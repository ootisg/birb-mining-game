#ifndef TILE_MAP_H
#define TILE_MAP_H

#include "game_object.h"

#define MAP_TILE_SIZE .0625f
#define MAP_GRID_WIDTH 100
#define MAP_GRID_HEIGHT 100

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

void update_tile_objs ();

#endif