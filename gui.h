#ifndef GUI_H
#define GUI_H

#include "game_object.h"

struct gui_reigon_data {
	unsigned char* bg_data;
	unsigned char* img_data;
	int img_width;
	int img_height;
	int valid;
	int mouse_status;
};

struct gui_component {
	game_object* ui;
	void* ui_data;
	layout_element* layout;
	linked_list* reigons;
	rectangle prev_bounds;
	struct gui_reigon_data* reigon_data;
	int num_reigons;
	void (*click_event) (struct gui_component* cpt, int index, int button, float x, float y);
	void (*hover_event) (struct gui_component* cpt, int index, float x, float y);
	void (*mouse_enter_event) (struct gui_component* cpt, int index, float x, float y);
	void (*mouse_exit_event) (struct gui_component* cpt, int index, float x, float y);
	void (*key_event) (struct gui_component* cpt, int index, char key);
	void (*reigon_init_func) (struct gui_component* cpt, int index);
	void (*render_func) (struct gui_component* cpt, int index);
};

typedef struct gui_component gui_component;
typedef struct gui_reigon_data gui_reigon_data;

void init_gui_component (void* ptr, char* layout, rectangle* bounds, char* bg);
void gui_component_init (game_object* obj);
void gui_component_frame_event (game_object* obj);
void gui_component_draw (game_object* obj);
void gui_component_hide (gui_component* cpt);
void gui_component_show (gui_component* cpt);

rectangle* get_layout_elem_by_index (int index);

#endif