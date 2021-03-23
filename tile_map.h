#ifndef TILE_MAP_H
#define TILE_MAP_H

#include "game_object.h"

#define MAP_TILE_SIZE .0625f
#define MAP_GRID_WIDTH 100
#define MAP_GRID_HEIGHT 100

#define MAX_TILE_ID 255

#define TILE_SKY_1 0

#define TILE_BG_1 1
#define TILE_BG_2 9

#define TILE_DIRT_1 2
#define TILE_DIRT_2 10

#define TILE_GRASS_1 3

#define TILE_COAL_1 4

/// A struct for map tiles, I think
struct map_tile {
	int x;
	int y;
	int id;
	rectangle bounds;
	game_object* tile_obj;
};

typedef struct map_tile map_tile;

/// Makes a map tile with default parameters at the given location ptr:
///		x, y, id are initialized to -1
///		bounds is initialized OOB according to x, y
///		tile_obj is initialzed to NULL
/// @param the location to put the new tile
void make_map_tile (void* ptr);

/// Initializes the map.
void init_map_tiles ();

/// Generates the given tile.
/// @param tile the tile to generate
void generate_tile (map_tile* tile);

/// Gets the map as a tile grid.
/// 	the tile [x, y] is located at index y * MAP_GRID_WIDTH + x
/// @return the tile grid
map_tile* get_tile_grid ();

/// Gets the map tile located at [x, y]
/// @param x the x-coordinate to get
/// @param y the y-coordinate to get
/// @return the map_tile at the coordinates [x, y]
map_tile* map_get_tile (int x, int y);

/// Gets the tileset sprite. The tiles are stored in the following way:
/// 	A tile's texture is stored as the nth frame of this sprite, where n is the tile's numeric id
///		e.g. dirt_1 has a numeric ID of 2, so its texture is frame 2 of this sprite
/// @return the sprite containing every tile
sprite* get_tileset ();

/// Gets the JSON object containing the properties for the given numeric tile ID.
/// 	Note: undefined tiles will give the default tile properties, but default properties are not given case-per-case in this way.
/// 	e.g. if the requested tile exists but does not define the property "hardness", the returned json_object will NOT have the default value for hardness.
///		However, if the requested tile does not exist, the json_object returned will have all defualt properties.
/// @param id the numeric tile id to search for
/// @return a json_object containing the properties of the given tile
json_object* get_tile_properties (int id);

/// Gets the property with the given name for the tile with the given numeric tile ID.
///		If the id is undefined or the property does not exist for the given tile, the default tile property is returned instead.
///	@param id the numeric tile id to search with
/// @param name the property name to search for
/// @return the property 'name' of the tile 'id'
void* get_tile_property (int id, char* name);

/// Gets the numeric tile id corresponding to the given tile name
/// @param the tile name to search with
/// @return the numeric tile id corresponding to the given name; -1 if no such tile exists
int tile_id_by_name (char* name);

/// Loads the tile properties. Called automatically by get_tile_properties, get_tile_property, and tile_id_by_name when needed. Does nothing if the tile properties are already loaded.
void load_tile_properties ();

/// Updates the game_object(s) used to display the map tiles. Must be called once per frame to properly display the map.
void update_tile_objs ();

#endif