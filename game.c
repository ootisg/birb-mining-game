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

gui_component* gm_inventory;
gui_component* gm_shop;

void game_init () {
	
	//For now, THE ORDER IS IMPORTANT BECAUSE IT DETERMINES RENDER ORDER
	//Make the inventory and the shop
	init_inventory ();
	printf ("INVENTORY PTR: %x\n", get_inventory ());
	init_shop ();
	gm_inventory = get_inventory ();
	gm_shop = get_shop ();
	
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
	
	//Get the inputs
	input_state* inputs = get_inputs ();
	
	//Handle toggling the inventory
	if (inputs->keys_pressed['e']) {
		if (gm_inventory->ui->x == -1 && gm_shop->ui->x == -1) {
			gui_component_show (gm_inventory);
		} else if (gm_inventory->ui->x != -1) {
			gui_component_hide (gm_inventory);
		}
	}
	
	//Handle toggling the shop (TODO change how the shop is accessed ingame)
	if (inputs->keys_pressed['q']) {
		if (gm_shop->ui->x == -1 && gm_inventory->ui->x == -1) {
			gui_component_show (gm_shop);
		} else if (gm_shop->ui->x != -1) {
			gui_component_hide (gm_shop);
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