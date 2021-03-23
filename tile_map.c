#include "tile_map.h"

#define STB_PERLIN_IMPLEMENTATION
#include "stb_perlin.h"

#include "stack.h"
#include "object_handler.h"
#include "viewport.h"
#include "game.h"

#include <stdio.h>
#include <stdlib.h>

#define NUM_TILE_OBJS 1024 //Does not need to be this large
#define GROUND_LEVEL 14

stack* tile_obj_stack;
stack* freed_tile_obj_stack;

sprite* tileset = NULL;

map_tile tile_grid[MAP_GRID_HEIGHT][MAP_GRID_WIDTH];

json_object* tile_properties = NULL;
json_object* default_properties;
hash_table* tile_properties_table;
hash_table* tile_name_table;

int seed_1;
float seed_2;

void make_map_tile (void* ptr) {
	
	map_tile* tile_ptr = (map_tile*)ptr;
	tile_ptr->x = -1;
	tile_ptr->y = -1;
	tile_ptr->id = -1;
	make_rectangle (&(tile_ptr->bounds), -1, -1, MAP_TILE_SIZE, MAP_TILE_SIZE);
	tile_ptr->tile_obj = NULL;
	
}

void init_map_tiles () {
	
	//Init the RNG state
	seed_1 = rand () % 256;
	seed_2 = (float)rand () / RAND_MAX;
	
	//Init the tile grid
	int wx, wy;
	for (wy = GROUND_LEVEL; wy < MAP_GRID_HEIGHT; wy++) {
		for (wx = 0; wx < MAP_GRID_WIDTH; wx++) {
			map_tile* curr = &(tile_grid[wy][wx]);
			curr->x = wx;
			curr->y = wy;
			generate_tile (curr);
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

void generate_tile (map_tile* tile) {
	
	//Get the coords
	int wx = tile->x;
	int wy = tile->y;
	
	//Generate the tile
	if (wy >= GROUND_LEVEL + 1) {
		if (stb_perlin_noise3_seed((float)wx / 8 + .5, (float)wy / 8 + .5, seed_2, 0, 0, 0, seed_1) >= 0) {
			tile->id = rand () > RAND_MAX / 2 ? tile_id_by_name ("bg_1") : tile_id_by_name ("bg_2");
		} else {
			//Soil
			tile->id = rand () > RAND_MAX / 2 ? tile_id_by_name ("dirt_1") : tile_id_by_name ("dirt_2");
			if (stb_perlin_noise3_seed((float)wx / 4 + .5, (float)wy / 4 + .5, seed_2 + 1.0f, 0, 0, 0, seed_1) >= .5) {
				tile->id = tile_id_by_name ("coal_1");
			}
		}
	} else if (wy == GROUND_LEVEL) {
		tile->id = tile_id_by_name ("grass_1");
	} else {
		tile->id = tile_id_by_name ("sky_1");
	}
	
}

//Helper function, not defined in header
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

void force_update_tile (map_tile* tile) {
	int x = tile->x;
	int y = tile->y;
	make_rectangle (&(tile->bounds), x * MAP_TILE_SIZE, y * MAP_TILE_SIZE, MAP_TILE_SIZE, MAP_TILE_SIZE);
	game_object* t_obj = tile->tile_obj;
	if (t_obj) {
		t_obj->x = x * MAP_TILE_SIZE;
		t_obj->y = y * MAP_TILE_SIZE;
		t_obj->width = MAP_TILE_SIZE;
		t_obj->height = MAP_TILE_SIZE;
		animation_handler_set_frame (&(t_obj->animator), tile->id);
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

//These functions do not require comments. Come to think of it, this comment isn't needed either.
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

void load_tile_properties () {
	
	//Stop if tile properties are already loaded
	if (tile_properties) {
		return;
	}
	
	//Load the JSON file
	tile_properties = read_json_file ("resources/config/tiles.json");
	
	//Init the default properties
	default_properties = json_get_object (tile_properties, "default");
	
	//Make the hash tables
	tile_properties_table = make_hash_table (malloc (sizeof (hash_table)));
	tile_name_table = make_hash_table (malloc (sizeof (hash_table)));
	
	//Fill the tile property tables from the JSON file
	char* num_str = malloc (10);
	int i;
	for (i = 0; i < MAX_TILE_ID; i++) {
		sprintf (num_str, "%d", i);
		json_object* curr = json_get_object (tile_properties, num_str);
		if (curr) {
			char* tile_name = json_get_data (curr, "name");
			int* stored_id = malloc (sizeof (int));
			*stored_id = i;
			hash_table_put (tile_properties_table, &i, sizeof (int), curr);
			hash_table_put (tile_name_table, tile_name, strlen (tile_name), stored_id);
		}
	}

}

json_object* get_tile_properties (int id) {
	
	//Make sure the tile properties are loaded
	load_tile_properties ();
	
	//Get the properties from the tile properties table
	json_object* properties = hash_table_get (tile_properties_table, &id, sizeof (int));
	return properties ? properties : default_properties;
	
}

void* get_tile_property (int id, char* name) {
	
	//Make sure the tile properties are loaded
	load_tile_properties ();
	
	//Get and return the relevant property
	json_object* properties = get_tile_properties (id);
	json_object* dat = json_get_object (properties, name);
	return dat ? dat->token_data : json_get_data (default_properties, name);
	
}

int tile_id_by_name (char* name) {
	
	//Make sure the tile properties are loaded
	load_tile_properties ();
	
	//Grab the tile ID from the tile ID table
	int* val = hash_table_get (tile_name_table, name, strlen (name));
	return val ? *val : -1;
	
}