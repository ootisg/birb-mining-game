#ifndef GAME_UI_H
#define GAME_UI_H

#include "gui.h"
#include "linked_list.h"

//------------------- Inventory ----------------------

struct inventory_data {
	int* items;
	int selected_index;
};

typedef struct inventory_data inventory_data;

void init_inventory ();
gui_component* get_inventory ();
int inventory_store_item (int id);
int inventory_remove_item (int id);
int inventory_query (int id);
void inventory_render_func (gui_component* cpt, int index);
void inventory_mouse_enter_func (struct gui_component* cpt, int index, float x, float y);
void inventory_mouse_exit_func (struct gui_component* cpt, int index, float x, float y);

//------------------- Shop ----------------------

// Current idea for the shop contents:
// Slot 1 contains beak upgrades
// Slot 2 contains misc. upgrades
// Slot 3 contains blueprints
// Slot 4 contains a (shuffled) resource
struct shop_data {
	int slot_a_data;
	int slot_b_data;
	int slot_c_data;
	int slot_d_data;
	int selected_index;
};

typedef struct shop_data shop_data;

void init_shop ();
gui_component* get_shop ();
void shop_render_func (gui_component* cpt, int index);
void shop_mouse_enter_func (struct gui_component* cpt, int index, float x, float y);
void shop_mouse_exit_func (struct gui_component* cpt, int index, float x, float y);
void shop_click_func (struct gui_component* cpt, int index, int button, float x, float y);

#endif