#include "viewport.h"
#include "inputs.h"

#include <stdlib.h>
#include <stdio.h>

rectangle* viewport = NULL;

float last_mouse_x = -1;
float last_mouse_y = -1;

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
	
}