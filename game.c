#include "game.h"
#include "birb.h"
#include "npc.h"
#include "geometry.h"
#include "viewport.h"
#include "tile_map.h"
#include "gui.h"
#include "game_ui.h"

#include <stdio.h>

point scratch_pt;

gui_component* inventory;

void game_init () {
	
	//For now, THE ORDER IS IMPORTANT BECAUSE IT DETERMINES RENDER ORDER
	//Make the inventory
	init_inventory ();
	inventory = get_inventory ();
	
	//Make the birb
	game_object* birb = make_birb ();
	
	//Initialize the NPCS
	init_npcs ();
	
	//Initialize the map tiles
	init_map_tiles ();
	
	//Do misc. initializations
	reveal_around_birb (birb);
	
}

void game_logic_loop () {
	
	input_state* inputs = get_inputs ();
	if (inputs->keys_pressed['e']) {
		if (inventory->ui->x == -1) {
			gui_component_show (inventory);
		} else {
			gui_component_hide (inventory);
		}
	}
	
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