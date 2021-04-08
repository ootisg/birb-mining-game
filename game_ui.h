#ifndef GAME_UI_H
#define GAME_UI_H

#include "gui.h"
#include "linked_list.h"

struct inventory_contents {
	linked_list* inventory_contents;
};

typedef struct inventory_contents inventory_contents;

void init_inventory ();
gui_component* get_inventory ();
int inventory_store_item (int id);
int inventory_remove_item (int id);
int inventory_query (int id);
void inventory_render_func (gui_component* cpt, int index);

#endif