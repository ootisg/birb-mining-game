#include "game_ui.h"

#include "sprite.h"

gui_component* inventory;

void init_inventory () {
	inventory = malloc (sizeof (gui_component));
	init_gui_component (inventory, "resources/config/inventory.json", make_rectangle (malloc (sizeof (rectangle)), 0, 0, 1, 1), "resources/sprites/inventory.png");
	inventory->ui_data = make_linked_list (malloc (sizeof (linked_list)));
	inventory->render_func = inventory_render_func;
	gui_component_hide (inventory);
}

gui_component* get_inventory () {
	return inventory;
}

int inventory_store_item (int id) {
	linked_list_node* curr = ((linked_list*)inventory->ui_data)->head;
	int index = 0;
	int found = 0;
	while (curr) {
		int* id_amt_pair = curr->node_data;
		if (id_amt_pair[0] == id) {
			id_amt_pair[1]++;
		}
		index++;
		curr = curr->next;
	}
	if (index <= 9) {
		//There's room to add the item to the inventory
		int* id_amt_pair = malloc (sizeof (int) * 2);
		linked_list_add ((linked_list*)inventory->ui_data, id_amt_pair, 2);
	}
}

int inventory_remove_item (int id) {
}

int inventory_query (int id) {
}

void inventory_render_func (gui_component* cpt, int index) {
	if (index > 0) {
		image_buffer_fill (cpt->reigon_data[index].img_data, cpt->reigon_data[index].img_width, cpt->reigon_data[index].img_height, 0x00000000);
		image_buffer_draw_string (cpt->reigon_data[index].img_data, cpt->reigon_data[index].img_width, cpt->reigon_data[index].img_height, 0, 0, "ITEM SLOT");
	}
}