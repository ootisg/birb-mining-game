#ifndef RENDER_H
#define RENDER_H

#include <math.h>

#include "geometry.h"
#include "lodepng.h"
#include "inputs.h"
#include "sprite.h"
#include "texture_mapping.h"

#define MAX_TEXTURES 1024
#define TEXTURE_SIZE 1024
#define TEXTURE_TILE_SIZE 8

#define MAX_OBJECTS 1048571 //Perhaps a bit excessive

/// Allocates space on a 'sprite texture' for an image of the given width and height, and returns a texture_mapping representing its location.
///		A 'sprite texture' is image data for an OpenGL texture where sprite data is stored.
/// @param width the width of the image
/// @param height the height of the image
/// @return the texture_mapping representing the image's location on a texture
texture_mapping* get_texture_mapping (int width, int height);

/// Gets the 'sprite texture' with the given ID.
///		A 'sprite texture' is image data for an OpenGL texture where sprite data is stored.
/// @param id the id of the texture
/// @return the requested sprite texture
char* get_sprite_texture (unsigned int id);

/// Helper method to allocate a new sprite texture.
/// @return the id of the newly allocated sprite texture
unsigned int make_sprite_texture ();

/// Checks if the specified space on the given sprite texture is available for use.
///		A 'sprite texture' is image data for an OpenGL texture where sprite data is stored.
/// @param tex_id the id of the sprite texture to check
/// @param x the x-coordinate of the bounding box to check, in px
/// @param y the y-coordinate of the bounding box to check, in px
/// @param width the width of the bounding box to check, in px
/// @param height the height of the bounding box to check, in px
/// @return non-zero if the space is available for use, 0 otherwise
int space_available (int tex_id, int x, int y, int width, int height);

/// Marks he specified space on the given sprite texture to be 'taken'
/// 	A 'sprite texture' is image data for an OpenGL texture where sprite data is stored.
///	@param tex_id the id of the texture to mark
/// @param x the x-coordinate of the bounding box to mark, in px
/// @param y the y-coordinate of the bounding box to mark, in px
/// @param width the width of the bounding box to mark, in px
/// @param height the height of the bounding box to mark, in px
void mark_space (int tex_id, int x, int y, int width, int height);

/// Initializes a rectangle at ptr to the rectangle bounds, snapped to the pixels of the sprite textures
/// @param ptr the location to initialize the new rectangle at
/// @param bounds the rectangle to snap
/// @return ptr cast to rectangle*
rectangle* snap_to_textures (rectangle* ptr, rectangle* bounds);

#endif