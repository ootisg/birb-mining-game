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

sprite* shop_birb_sprite;

game_object* shop_birb;

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
	
	//Initialize the NPC
	init_npcs ();
	//Initialize the shop NPCS
	shop_birb_sprite = make_sprite ("resources/sprites/shoppy_birb.png");
	shop_birb = alloc_npc (1);
	shop_birb->x = birb->x + .5;
	shop_birb->y = birb->y;
	shop_birb->width = MAP_TILE_SIZE * 2;
	shop_birb->height = MAP_TILE_SIZE * 3;
	shop_birb->object_data = make_npc_data (malloc (sizeof (npc_data)), "shop", (void*)0);
	shop_birb->sprite = shop_birb_sprite;
	generate_hitbox (shop_birb);
	
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
		toggle_menu ();
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

void open_shop () {
	gui_component_show (gm_shop);
}

void toggle_menu () {
	if (gm_inventory->ui->x == -1 && gm_shop->ui->x == -1) {
		gui_component_show (gm_inventory);
	} else if (gm_inventory->ui->x != -1) {
		gui_component_hide (gm_inventory);
	} else if (gm_shop->ui->x != -1) {
		gui_component_hide (gm_shop);
	}
}