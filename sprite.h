#ifndef SPRITE_H
#define SPRITE_H

#include <stdio.h>

#include "geometry.h"
#include "lodepng.h"
#include "texture_mapping.h"
#include "render.h"
#include "json.h"
#include "layout.h"
#include "main.h"

/// A struct representing a sprite (woefully underdocumented)
struct sprite {
	unsigned char* image_data;
	unsigned int texture_id;
	unsigned int frame_count;
	unsigned int width_px;
	unsigned int height_px;
	float width;
	float height;
	texture_mapping* mapping;
	rectangle* frames;
};

typedef struct sprite sprite;

/// Makes a sprite from the given image filepath (MUST BE A PNG)
/// @param filepath the filepath to get the image from
/// @return the resulting sprite
sprite* make_sprite (char* filepath);

/// Makes a sprite from the given json filepath; a force-override image filepath can also be passed, and will be ignored if NULL.
/// @param json_path the path to the json file to load from; ignored if set to NULL
/// @param sprite_path the (optional) path to the image (MUST BE A PNG); will be ignored if set to NULL
/// @return the resulting sprite
sprite* make_sprite_from_json (char* json_path, char* sprite_path);

/// Draws a sprite at the given coordinates.
/// @param spr the sprite to draw
/// @param x the x-coordinate to draw the sprite at (0 = screen left, 1 = one screen height to the right)
/// @param y the y-coordinate to draw the sprite at (0 = screen top, 1 = screen bottom)
/// @param frame the frame of the sprite to draw
void draw_sprite (sprite* spr, double x, double y, int frame);

/// Transfers the given image data to a 'sprite texture', defined by the given texture_mapping.
/// @param texture_mapping the mapping to use
/// @param source the source image data to transfer
/// @param source_width the width of the source image
/// @param source_height the height of the source image
void transfer_to_texture (texture_mapping* map, unsigned char* source, int source_width, int source_height);

#endif