#include <stdio.h>
#include <stdlib.h>

#include <GL/glew.h>
#include <GL/glut.h>

#include "render.h"
#include "main.h"

int texture_count = 0;
char* textures[MAX_TEXTURES];
char* texture_availability_map[MAX_TEXTURES];
unsigned int last_add_index;
unsigned int last_add_x;
unsigned int last_add_y;

texture_mapping* get_texture_mapping (int width_px, int height_px) {
	
	//Make the mapping object for this texture
	texture_mapping* map = malloc (sizeof (texture_mapping));
	map->tex_id = 0;
	
	//Find an empty space for this texture
	if (texture_count == 0) {
		make_sprite_texture ();
	}
	int wt, wx, wy;
	int max_width = TEXTURE_SIZE / TEXTURE_TILE_SIZE;
	for (wt = 0; wt < MAX_TEXTURES; wt++) {
		for (wy = 0; wy < max_width; wy++) {
			for (wx = 0; wx < max_width; wx++) {
				//TODO use a more sophisticated algorithm
				if (space_available (wt, wx * TEXTURE_TILE_SIZE, wy * TEXTURE_TILE_SIZE, width_px, height_px)) {
					//Set the bounds accordingly
					(&(map->bounds))->x = (float)(wx * TEXTURE_TILE_SIZE) / TEXTURE_SIZE;
					(&(map->bounds))->y = (float)(wy * TEXTURE_TILE_SIZE) / TEXTURE_SIZE;
					(&(map->bounds))->width = (float)width_px / TEXTURE_SIZE;
					(&(map->bounds))->height = (float)height_px / TEXTURE_SIZE;
					//Mark the texture space as taken
					mark_space (wt, wx * TEXTURE_TILE_SIZE, wy * TEXTURE_TILE_SIZE, width_px, height_px);
					//Exit out of the loop
					goto BREAKPT;
				}
			}
		}
		if (wt == texture_count - 1) {
			make_sprite_texture ();
		}
	}
	BREAKPT:
	return map;
}

char* get_sprite_texture (unsigned int id) {
	return textures[id];
}

unsigned int make_sprite_texture () {
	
	//Allocate the image data and availability map for the texture
	char* img_data = malloc (TEXTURE_SIZE * TEXTURE_SIZE * sizeof (int));
	int av_size = (TEXTURE_SIZE / TEXTURE_TILE_SIZE) * (TEXTURE_SIZE / TEXTURE_TILE_SIZE) * sizeof (char);
	char* av_data = malloc (av_size);
	
	//Fill the availability map
	int i;
	for (i = 0; i < av_size; i++) {
		av_data[i] = 1;
	}
	
	//Store the new texture and availability map, then return the new texture count
	texture_availability_map[texture_count] = av_data;
	textures[texture_count++] = img_data;
	return texture_count;
}

int space_available (int tex_id, int x, int y, int width, int height) {
	
	//Get the proper availability map
	char* av_map = texture_availability_map [tex_id];
	
	//Compute the tiles overlapped by the bounding box, and the width of the texture in tiles
	int tile_x = x / TEXTURE_TILE_SIZE;
	int tile_y = y / TEXTURE_TILE_SIZE;
	int tile_width = width / TEXTURE_TILE_SIZE;
	int tile_height = height / TEXTURE_TILE_SIZE;
	int max_width = TEXTURE_SIZE / TEXTURE_TILE_SIZE;
	int av_map_width = TEXTURE_SIZE / TEXTURE_TILE_SIZE;
	
	//Iterate over the relevant tiles to check if any are taken
	int wx, wy;
	for (wx = 0; wx < tile_width; wx++) {
		for (wy = 0; wy < tile_height; wy++) {
			int cx = tile_x + wx;
			int cy = tile_y + wy;
			if (cx >= max_width || cy >= max_width || !av_map[cy * av_map_width + cx]) {
				return 0;
			}
		}
	}
	
	//Return 'true' if none of the tiles are taken
	return 1;
}

void mark_space (int tex_id, int x, int y, int width, int height) {
	
	//Get the proper availability map
	char* av_map = texture_availability_map [tex_id];
	
	//Compute the tiles overlapped by the bounding box, and the width of the texture in tiles
	int tile_x = x / TEXTURE_TILE_SIZE;
	int tile_y = y / TEXTURE_TILE_SIZE;
	int tile_width = width / TEXTURE_TILE_SIZE;
	int tile_height = height / TEXTURE_TILE_SIZE;
	int av_map_width = TEXTURE_SIZE / TEXTURE_TILE_SIZE;
	
	//Iterate over the relevant tiles and mark them all as 'taken'
	int wx, wy;
	for (wx = 0; wx < tile_width; wx++) {
		for (wy = 0; wy < tile_height; wy++) {
			int cx = tile_x + wx;
			int cy = tile_y + wy;
			av_map[cy * av_map_width + cx] = (char)0;
		}
	}
}

rectangle* snap_to_textures (rectangle* ptr, rectangle* bounds) {
	rectangle* r_ptr = (rectangle*)ptr;
	float new_x = round (bounds->x * TEXTURE_SIZE) / TEXTURE_SIZE;
	float new_y = round (bounds->y * TEXTURE_SIZE) / TEXTURE_SIZE;
	float new_width = round (bounds->width * TEXTURE_SIZE) / TEXTURE_SIZE;
	float new_height = round (bounds->height * TEXTURE_SIZE) / TEXTURE_SIZE;
	make_rectangle (r_ptr, new_x, new_y, new_width, new_height);
	return r_ptr;
}