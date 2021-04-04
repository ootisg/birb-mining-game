#include "gui.h"

#include "object_handler.h"

void init_gui_component (void* ptr, char* layout) {
	
	//Make the GUI component
	gui_component* cpt = (gui_component*)ptr;
	
	//Load the layout from the file
	json_object* layout_json = read_json_file (layout);
	cpt->layout = make_layout (malloc (sizeof (layout_element)), layout_json);
	
	//Initialize the game_object interface
	cpt->ui = make_game_object (malloc (sizeof (game_object)), "gui_component");
	cpt->ui->init_call = gui_component_init;
	cpt->ui->game_logic_call = gui_component_frame_event;
	cpt->ui->draw_call = gui_component_draw;
	declare_game_object (get_global_object_handler (), cpt->ui);
	
	//Set the event calls to NULL
	cpt->click_event = NULL;
	cpt->hover_event = NULL;
	cpt->mouse_enter_event = NULL;
	cpt->mouse_exit_event = NULL;
	cpt->key_event = NULL;
	cpt->render_func = NULL;
	
}

void gui_component_init (game_object* obj) {
	
}

void gui_component_frame_event (game_object* obj) {
	
}

void gui_component_draw (game_object* obj) {
	
}

rectangle* get_layout_elem_by_index (int index) {
}