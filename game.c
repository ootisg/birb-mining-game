#include "game.h"
#include "birb.h"
#include "npc.h"
#include "geometry.h"
#include "viewport.h"
#include "tile_map.h"

#include <stdio.h>

point scratch_pt;

void game_init () {
	
	game_object* birb = make_birb ();
	init_map_tiles ();
	init_npcs ();
	reveal_around_birb (birb);
	
	game_object* temp = alloc_npc ();
	temp->x = 0;
	temp->y = 0;
	temp->width = .5;
	temp->height = .5;
	//temp->depth = 5.0f;
	temp->sprite = make_sprite ("testure.png");
	
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