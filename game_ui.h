#ifndef GAME_UI_H
#define GAME_UI_H

#include "gui.h"
#include "linked_list.h"

//------------------- Inventory ----------------------

struct inventory_contents {
	int* items;
};

typedef struct inventory_contents inventory_contents;

void init_inventory ();
gui_component* get_inventory ();
int inventory_store_item (int id);
int inventory_remove_item (int id);
int inventory_query (int id);
void inventory_render_func (gui_component* cpt, int index);

//------------------- Shop ----------------------

// Current idea for the shop contents:
// Slot 1 contains beak upgrades
// Slot 2 contains misc. upgrades
// Slot 3 contains blueprints
// Slot 4 contains a (shuffled) resource
struct shop_contents {
	int slot_a_data;
	int slot_b_data;
	int slot_c_data;
	int slot_d_data;
};

typedef struct shop_contents shop_contents;

void init_shop ();
gui_component* get_shop ();
void shop_render_func (gui_component* cpt, int index);

#endif