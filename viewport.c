#include "viewport.h"
#include "inputs.h"
#include "tile_map.h"

#include <stdlib.h>
#include <stdio.h>

rectangle* viewport = NULL;

float last_mouse_x = -1;
float last_mouse_y = -1;
float min_scroll_x = 0;
float min_scroll_y = 0;
float max_scroll_x = MAP_GRID_WIDTH * MAP_TILE_SIZE;
float max_scroll_y = MAP_GRID_HEIGHT * MAP_TILE_SIZE;

void transform_to_viewport (rectangle* viewport, point* pt) {
	
	//I think this will do it?
	pt->x = (pt->x - viewport->x) / viewport->width;
	pt->y = (pt->y - viewport->y) / viewport->height;
	
}

rectangle* get_viewport () {
	
	//Make the viewport if it's not made already
	if (viewport == NULL) {
		viewport = make_rectangle (malloc (sizeof (rectangle)), 0, 0, 1, 1);
	}
	//Return the current viewport
	return viewport;
	
}

void update_viewport () {
	
	//Make the viewport if it's not made already
	if (viewport == NULL) {
		viewport = make_rectangle (malloc (sizeof (rectangle)), 0, 0, 1, 1);
	}
	
	//Check for user input
	input_state* inputs = get_inputs ();
	if (inputs->mouse_buttons_down[2]) {
		if (last_mouse_x != -1 && last_mouse_y != -1) {
			float x_diff = inputs->mouse_x - last_mouse_x;
			float y_diff = inputs->mouse_y - last_mouse_y;
			viewport->x -= x_diff;
			viewport->y -= y_diff;
		}
		last_mouse_x = inputs->mouse_x;
		last_mouse_y = inputs->mouse_y;
	} else {
		last_mouse_x = -1;
		last_mouse_y = -1;
	}
	
	//Bound the viewport to its scroll limits
	bind_viewport ();
	
}

void bind_viewport () {
	viewport->x = viewport->x <= min_scroll_x ? min_scroll_x : (viewport->x);
	viewport->y = viewport->y <= min_scroll_y ? min_scroll_y : (viewport->y);
	viewport->x = viewport->x + viewport->width > max_scroll_x ? (max_scroll_x - viewport->width) : (viewport->x);
	viewport->y = viewport->y + viewport->height > max_scroll_y ? (max_scroll_y - viewport->height) : (viewport->y);
}