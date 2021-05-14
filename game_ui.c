#include "game_ui.h"

#include "sprite.h"

//------------------- Inventory ----------------------

gui_component* inventory;
json_object* item_data;
char string_buffer[128];
char* item_names[256];

void init_inventory () {
	//Make the inventory gui component
	inventory = malloc (sizeof (gui_component));
	init_gui_component (inventory, "resources/config/inventory.json", make_rectangle (malloc (sizeof (rectangle)), 0, 0, 1, 1), "resources/sprites/inventory.png");
	
	//Allocate and fill the inventory data
	inventory->ui_data = malloc (sizeof (inventory_data));
	((inventory_data*)inventory->ui_data)->items = malloc (sizeof (int) * 2 * 10);
	((inventory_data*)inventory->ui_data)->selected_index = -1;
	int i;
	for (i = 0; i < 20; i++) {
		((inventory_data*)inventory->ui_data)->items[i] = -1;
	}
	
	//Setup additional properties
	inventory->render_func = inventory_render_func;
	inventory->mouse_enter_event = inventory_mouse_enter_func;
	inventory->mouse_exit_event = inventory_mouse_exit_func;
	gui_component_hide (inventory);
	//inventory_store_item (0);
	item_data = read_json_file ("resources/config/items.json");
	//TODO temporary solution
	item_names[0] = "Coal";
	item_names[1] = "Copper";
	item_names[2] = "Gold";
	item_names[3] = "Ruby";
	
}

gui_component* get_inventory () {
	return inventory;
}

void inventory_redraw_slot (int slot) {
	gui_reigon_data* rg_data = &(inventory->reigon_data[slot + 1]); //Gets the reigon data for the correct inventory slot
	rg_data->valid = 0;
}

int inventory_store_item (int id) {
	int* contents = ((inventory_data*)inventory->ui_data)->items;
	int i;
	for (i = 0; i < 10; i++) {
		int idx = i * 2;
		if (contents[idx] == id) {
			contents[idx + 1]++;
			inventory_redraw_slot (i);
			return 1;
		}
		if (contents [idx] == -1) {
			contents [idx] = id;
			contents [idx + 1] = 1;
			inventory_redraw_slot (i);
			return 1;
		}
	}
	return 0;
}

int inventory_remove_item (int id) {
}

int inventory_query (int id) {
}

void inventory_render_func (gui_component* cpt, int index) {
	
	//Get the associated item slot
	int* items = ((inventory_data*)inventory->ui_data)->items;
	
	//Draw the slot
	if (index > 0 && items[(index - 1) * 2] != -1) {
		
		//Fill the cell appropriately
		if (((inventory_data*)inventory->ui_data)->selected_index == index) {
			image_buffer_fill (cpt->reigon_data[index].img_data, cpt->reigon_data[index].img_width, cpt->reigon_data[index].img_height, 0xFF777777);
		} else {
			image_buffer_fill (cpt->reigon_data[index].img_data, cpt->reigon_data[index].img_width, cpt->reigon_data[index].img_height, 0x00000000);
		}
		
		//Draw the text
		sprintf (&string_buffer[0], "%s x %d", item_names[items [(index - 1) * 2]], items [(index - 1) * 2 + 1]);
		image_buffer_draw_string (cpt->reigon_data[index].img_data, cpt->reigon_data[index].img_width, cpt->reigon_data[index].img_height, 2, 0, &string_buffer[0]);
		
	}
	
}

void inventory_mouse_enter_func (struct gui_component* cpt, int index, float x, float y) {
	
	if (index != 0) {
		inventory_data* inv_data = (inventory_data*)inventory->ui_data;
		inv_data->selected_index = index;
		(&(inventory->reigon_data[index]))->valid = 0;
	}
	
}

void inventory_mouse_exit_func (struct gui_component* cpt, int index, float x, float y) {
	
	if (index != 0) {
		inventory_data* inv_data = (inventory_data*)inventory->ui_data;
		if (inv_data->selected_index == index) {
			inv_data->selected_index = -1;
		}
		(&(inventory->reigon_data[index]))->valid = 0;
	}
	
}

//------------------- Shop ----------------------

gui_component* shop;

void init_shop () {
	
	//Make the shop gui component
	shop = malloc (sizeof (gui_component));
	init_gui_component (shop, "resources/config/shop.json", make_rectangle (malloc (sizeof (rectangle)), 0, 0, 1, 1), "resources/sprites/shop.png");
	
	//Allocate and setup the shop's data
	shop_data* contents = malloc (sizeof (shop_data));
	shop->ui_data = contents;
	contents->slot_a_data = 0;
	contents->slot_b_data = 0;
	contents->slot_c_data = 0;
	contents->slot_d_data = 0;
	
	//Setup additional properties
	shop->render_func = shop_render_func;
	shop->mouse_enter_event = shop_mouse_enter_func;
	shop->mouse_exit_event = shop_mouse_exit_func;
	gui_component_hide (shop);
	
}

gui_component* get_shop () {
	
	//Nothing special here
	return shop;
	
}

void shop_render_func (gui_component* cpt, int index) {
	
	//Get the shop data
	shop_data* shop_dat = (shop_data*)shop->ui_data;
	
	//Render the menu selection
	if (index > 0) {
		if (shop_dat->selected_index == index) {
			image_buffer_fill (cpt->reigon_data[index].img_data, cpt->reigon_data[index].img_width, cpt->reigon_data[index].img_height, 0xFF777777);
		} else {
			image_buffer_fill (cpt->reigon_data[index].img_data, cpt->reigon_data[index].img_width, cpt->reigon_data[index].img_height, 0x00000000);
		}
	}
	
}

void shop_mouse_enter_func (struct gui_component* cpt, int index, float x, float y) {
	
	if (index != 0) {
		shop_data* shop_dat = (shop_data*)shop->ui_data;
		shop_dat->selected_index = index;
		(&(shop->reigon_data[index]))->valid = 0;
	}
	
}

void shop_mouse_exit_func (struct gui_component* cpt, int index, float x, float y) {
	
	if (index != 0) {
		shop_data* shop_dat = (shop_data*)shop->ui_data;
		if (shop_dat->selected_index == index) {
			shop_dat->selected_index = -1;
		}
		(&(shop->reigon_data[index]))->valid = 0;
	}
	
}