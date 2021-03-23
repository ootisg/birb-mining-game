#include "tile_map.h"

#define STB_PERLIN_IMPLEMENTATION
#include "stb_perlin.h"

#include "stack.h"
#include "object_handler.h"
#include "viewport.h"
#include "game.h"

#include <stdio.h>

#define NUM_TILE_OBJS 1024 //Does not need to be this large
#define GROUND_LEVEL 14

stack* tile_obj_stack;
stack* freed_tile_obj_stack;

sprite* tileset = NULL;

map_tile tile_grid[MAP_GRID_HEIGHT][MAP_GRID_WIDTH];

void make_map_tile (void* ptr) {
	
	map_tile* tile_ptr = (map_tile*)ptr;
	tile_ptr->x = -1;
	tile_ptr->y = -1;
	tile_ptr->id = -1;
	make_rectangle (&(tile_ptr->bounds), -1, -1, MAP_TILE_SIZE, MAP_TILE_SIZE);
	tile_ptr->tile_obj = NULL;
	
}

void init_map_tiles () {
	
	//Init the tile grid
	int wx, wy;
	for (wy = GROUND_LEVEL; wy < MAP_GRID_HEIGHT; wy++) {
		for (wx = 0; wx < MAP_GRID_WIDTH; wx++) {
			map_tile* curr = &(tile_grid[wy][wx]);
			curr->x = wx * MAP_TILE_SIZE;
			curr->y = wy * MAP_TILE_SIZE;
			if (wy < GROUND_LEVEL + 2 || stb_perlin_noise3_seed((float)wx / 8 + .5, (float)wy / 8 + .5, 0.0f, 0, 0, 0, 69) >= 0) {
				curr->id = 1;
			} else {
				curr->id = 0;
			}
		}
	}
	
	//Init the map tile objects
	tile_obj_stack = init_stack (sizeof (game_object*));
	freed_tile_obj_stack = init_stack (sizeof (game_object*));
	int i;
	for (i = 0; i < NUM_TILE_OBJS; i++) {
		game_object* tile_obj = make_game_object (malloc (sizeof (game_object)), "map_tile");
		tile_obj->draw_call = viewport_draw;
		declare_game_object (get_global_object_handler (), tile_obj);
		tile_obj->x = -1;
		tile_obj->y = -1;
		tile_obj->sprite = get_tileset ();
		animation_handler_set_properties (&(tile_obj->animator), ANIMATION_HANDLER_STILL_FRAME, 1);
		push (tile_obj_stack, &tile_obj);
		push (freed_tile_obj_stack, &tile_obj);
	}
	
}

void assign_tile_render (int x, int y) {
	map_tile* tile = map_get_tile (x, y);
	make_rectangle (&(tile->bounds), x * MAP_TILE_SIZE, y * MAP_TILE_SIZE, MAP_TILE_SIZE, MAP_TILE_SIZE);
	if (!tile->tile_obj) {
		game_object* t_obj = *((game_object**)pop (freed_tile_obj_stack));
		t_obj->x = x * MAP_TILE_SIZE;
		t_obj->y = y * MAP_TILE_SIZE;
		t_obj->width = MAP_TILE_SIZE;
		t_obj->height = MAP_TILE_SIZE;
		animation_handler_set_frame (&(t_obj->animator), tile->id);
		tile->tile_obj = t_obj;
	}
}

void update_tile_objs () {
	
	//Remove all tile objects that are no longer in-bounds; a removed tile has a position of (-1, -1)
	game_object** objs_ptr = (game_object**)tile_obj_stack->data->data; //Same thing here ugh
	int i;
	for (i = 0; i < NUM_TILE_OBJS; i++) {
		game_object* obj_ptr = objs_ptr[i];
		if (obj_ptr->x != -1 && obj_ptr->y != -1) {
			int x_idx = obj_ptr->x / MAP_TILE_SIZE;
			int y_idx = obj_ptr->y / MAP_TILE_SIZE;
			map_tile* tile = map_get_tile (x_idx, y_idx);
			if (&(tile->bounds) && tile->tile_obj && !rectangle_overlaps (&(tile->bounds), get_viewport ())) {
				obj_ptr->x = -1;
				obj_ptr->y = -1;
				push (freed_tile_obj_stack, &obj_ptr);
				tile->tile_obj = NULL;
			}
		}
	}
	
	//Assign tile objects to all tiles in view
	rectangle* v = get_viewport ();
	float start_x = (int)(v->x / MAP_TILE_SIZE) * MAP_TILE_SIZE;
	float start_y = (int)(v->y / MAP_TILE_SIZE) * MAP_TILE_SIZE;
	float end_pt_x = v->x + v->width;
	float end_pt_y = v->y + v->height;
	float wx, wy;
	for (wx = start_x; wx < end_pt_x; wx += MAP_TILE_SIZE) {
		for (wy = start_y; wy < end_pt_y; wy += MAP_TILE_SIZE) {
			int x_idx = (int)(wx / MAP_TILE_SIZE);
			int y_idx = (int)(wy / MAP_TILE_SIZE);
			assign_tile_render (x_idx, y_idx);
		}
	}
	
}

map_tile* get_tile_grid () {
	return &tile_grid[0][0];
}

map_tile* map_get_tile (int x, int y) {
	return &(tile_grid[y][x]);
}

sprite* get_tileset () {
	if (!tileset) {
		tileset = make_sprite_from_json ("resources/sprites/config/tileset.json", NULL);
	}
	return tileset;
}