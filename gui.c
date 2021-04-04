#include "gui.h"

#include "object_handler.h"

void init_gui_component (void* ptr, char* layout, rectangle* bounds, char* bg) {
	
	//Make the GUI component
	gui_component* cpt = (gui_component*)ptr;
	
	//Load the layout from the file
	json_object* layout_json = read_json_file (layout);
	cpt->layout = make_layout (malloc (sizeof (layout_element)), layout_json);
	cpt->reigons = get_layout_reigons (cpt->layout, make_rectangle (malloc (sizeof (rectangle)), 0, 0, 1, 1));
	
	//Initialize the image data
	cpt->reigon_data = malloc (sizeof (gui_reigon_data) * cpt->reigons->size);
	linked_list_node* curr = cpt->reigons->head;
	int index = 0;
	while (curr) {
		rectangle* bounds = (rectangle*)curr->node_data;
		printf ("HIA\n");
		print_rectangle (bounds);
		int buffer_width = (int)(bounds->width * TEXTURE_SIZE);
		int buffer_height = (int)(bounds->height * TEXTURE_SIZE);
		cpt->reigon_data[index].img_data = malloc (sizeof (int) * buffer_width * buffer_height);
		cpt->reigon_data[index].mouse_status = 0;
		curr = curr->next;
		index++;
	}
	
	//Initialize the game_object interface
	cpt->ui = make_game_object (malloc (sizeof (game_object)), "gui_component");
	cpt->ui->init_call = gui_component_init;
	cpt->ui->game_logic_call = gui_component_frame_event;
	cpt->ui->draw_call = gui_component_draw;
	declare_game_object (get_global_object_handler (), cpt->ui);
	cpt->ui->sprite = make_sprite (bg);
	
	//Set the event calls to NULL
	cpt->click_event = NULL;
	cpt->hover_event = NULL;
	cpt->mouse_enter_event = NULL;
	cpt->mouse_exit_event = NULL;
	cpt->key_event = NULL;
	cpt->reigon_init_func = NULL;
	cpt->render_func = NULL;
	
}

void gui_component_init (game_object* obj) {
	default_init (obj);
}

void gui_component_frame_event (game_object* obj) {
	
}

void gui_component_draw (game_object* obj) {
	default_draw (obj);
}

rectangle* get_layout_elem_by_index (int index) {
}