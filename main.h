#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>

#include <GL/glew.h>
#include <GL/glut.h>

#include "geometry.h"
#include "lodepng.h"
#include "inputs.h"
#include "render.h"
#include "sprite.h"
#include "stack.h"

#define RENDER_OBJECT_SIZE 24

GLfloat* alloc_render_object ();
void refresh_sprite_textures ();
int get_frame_count ();
int get_frame_time_ms ();

#endif