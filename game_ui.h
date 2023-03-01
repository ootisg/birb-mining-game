#ifndef GAME_UI_H
#define GAME_UI_H

#include "gui.h"
#include "npc.h"
#include "tile_map.h"
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

struct shop_slot_buffer {
	int slot;
	int is_loaded;
	int item_price;
	unsigned char* image;
};

typedef struct shop_data shop_data;
typedef struct shop_slot_buffer shop_slot_buffer;

void init_shop_gui ();
void init_shop_npc (double x, double y);
gui_component* get_shop_gui ();
void shop_render_func (gui_component* cpt, int index);
void shop_mouse_enter_func (struct gui_component* cpt, int index, float x, float y);
void shop_mouse_exit_func (struct gui_component* cpt, int index, float x, float y);
void shop_click_func (struct gui_component* cpt, int index, int button, float x, float y);
game_object* get_shop_npc ();

//Toggles the opened menu
void toggle_menu ();

//Opens the shop menu
void open_shop ();

//----------------- Shop Bar ---------------------

void init_shop_bar_gui ();
gui_component* get_shop_bar_gui ();
void open_shop_bar_gui ();
void shop_bar_render_func (gui_component* cpt, int index);
void shop_bar_mouse_enter_func (struct gui_component* cpt, int index, float x, float y);
void shop_bar_mouse_exit_func (struct gui_component* cpt, int index, float x, float y);
void shop_bar_click_func (struct gui_component* cpt, int index, int button, float x, float y);

#endif