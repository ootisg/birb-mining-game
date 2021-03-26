#include "game.h"
#include "birb.h"
#include "geometry.h"
#include "viewport.h"
#include "tile_map.h"

#include <stdio.h>

point scratch_pt;

void game_init () {
	
	make_birb ();
	//make_break_anim ();
	//load_tile_properties ();
	init_map_tiles ();
	
}

void game_logic_loop () {
	
}

void viewport_draw (game_object* obj) {
	
	//Transform the object's coords to viewport coords
	double orig_x = obj->x;
	double orig_y = obj->y;
	scratch_pt.x = obj->x;
	scratch_pt.y = obj->y;
	transform_to_viewport (get_viewport (), &scratch_pt);
	obj->x = scratch_pt.x;
	obj->y = scratch_pt.y;
	
	//Draw the object and set its coordinates back to normal
	default_draw (obj);
	obj->x = orig_x;
	obj->y = orig_y;
	
}