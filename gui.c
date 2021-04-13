#include "gui.h"

#include "object_handler.h"

void init_gui_component (void* ptr, char* layout, rectangle* bounds, char* bg) {
	
	//Make the GUI component
	gui_component* cpt = (gui_component*)ptr;
	
	//Initialize the game_object interface
	cpt->ui = make_game_object (malloc (sizeof (game_object)), "gui_component");
	cpt->ui->init_call = gui_component_init;
	cpt->ui->game_logic_call = gui_component_frame_event;
	cpt->ui->draw_call = gui_component_draw;
	declare_game_object (get_global_object_handler (), cpt->ui);
	cpt->ui->sprite = make_sprite_from_json (layout, bg);
	cpt->ui->object_data = cpt;
	
	//Calculate the layout
	layout_element* layout_obj = make_layout (malloc (sizeof (layout_element)), read_json_file (layout));
	rectangle* unit_rectangle = make_rectangle (malloc (sizeof (rectangle)), 0, 0, 1, 1);
	cpt->reigons = get_layout_reigons (layout_obj, unit_rectangle);
	free (unit_rectangle);
	
	//Initialize the reigon data
	cpt->reigon_data = malloc (sizeof (gui_reigon_data) * cpt->ui->sprite->frame_count);
	
	//Initialize the image data
	cpt->num_reigons = cpt->ui->sprite->frame_count;
	int i;
	for (i = 0; i < cpt->ui->sprite->frame_count; i++) {
		int buffer_width;
		int buffer_height;
		get_sprite_image_data (cpt->ui->sprite, i, &(cpt->reigon_data[i].bg_data), &buffer_width, &buffer_height);
		cpt->reigon_data[i].img_data = malloc (sizeof (int) * buffer_width * buffer_height);
		image_buffer_fill (cpt->reigon_data[i].img_data, buffer_width, buffer_height, 0x00000000);
		cpt->reigon_data[i].img_width = buffer_width;
		cpt->reigon_data[i].img_height = buffer_height;
		cpt->reigon_data[i].valid = 0;
		cpt->reigon_data[i].mouse_status = 0;
	}
	
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
	obj->x = .25;
	obj->y = .25;
	obj->width = .5;
	obj->height = .5;
}

void gui_component_frame_event (game_object* obj) {
	
	//Get the inputs
	input_state* inputs = get_inputs ();
	
	if (inputs->mouse_x != -1 && inputs->mouse_y != -1) {
		
		//Calculate the scaled mouse inputs
		float mouse_x = inputs->mouse_x;
		float mouse_y = inputs->mouse_y;
		//Displace to the correct position
		mouse_x -= obj->x;
		mouse_y -= obj->y;
		//Scale to the correct size
		mouse_x /= obj->width;
		mouse_y /= obj->height;
		
		//Check mouse inputs
		gui_component* cpt = (gui_component*)obj->object_data;
		linked_list* bounds_list = cpt->reigons;
		linked_list_node* curr = bounds_list->head;
		int index = 0;
		while (curr) {
			
			//Get the current bounds and reigon data
			rectangle* r = curr->node_data;
			gui_reigon_data* rg_data = &(cpt->reigon_data[index]);
			
			//Calculate mouse event stuff
			int mouse_inside = rectangle_contains (r, mouse_x, mouse_y);
			float cpt_mouse_x = (mouse_x - r->x) / r->width;
			float cpt_mouse_y = (mouse_y - r->y) / r->height;
			
			//Call events as needed
			if (mouse_inside) {
				
				//Call the mouse entered event
				if (!rg_data->mouse_status && cpt->mouse_enter_event) {
					cpt->mouse_enter_event (cpt, index, cpt_mouse_x, cpt_mouse_y);
				}
				
				//Call the mouse hovered event
				if (cpt->hover_event) {
					cpt->hover_event (cpt, index, cpt_mouse_x, cpt_mouse_y);
				}
				
				//Call the mouse clicked event
				if (cpt->click_event) {
					int i = 0;
					for (i = 0; i < 4; i++) {
						if (inputs->mouse_buttons_down[i]) {
							cpt->click_event (cpt, index, i, cpt_mouse_x, cpt_mouse_y);
						}
					}
				}
				
				//Update the mouse inside status
				rg_data->mouse_status = 1;
				
			} else {
				
				//Call the mouse exited event
				if (rg_data->mouse_status && cpt->mouse_exit_event) {
					cpt->mouse_exit_event (cpt, index, cpt_mouse_x, cpt_mouse_y);
				}
				
				//Update the mouse inside status
				rg_data->mouse_status = 0;
				
			}
			
			//Update stuff for the next loop iteration
			curr = curr->next;
			index++;
			
		}
	}
	
}

void gui_component_draw (game_object* obj) {
	
	//Draw the frames
	gui_component* cpt = (gui_component*)obj->object_data;
	int i;
	for (i = 0; i < cpt->num_reigons; i++) {
		gui_reigon_data* rg = &(cpt->reigon_data[i]);
		if (!rg->valid) {
			if (cpt->render_func) {
				cpt->render_func (cpt, i);
			}
			draw_to_sprite (obj->sprite, rg->bg_data, i, 0, 0, rg->img_width, rg->img_height);
			draw_to_sprite (obj->sprite, rg->img_data, i, 0, 0, rg->img_width, rg->img_height);
			rg->valid = 1;
		}
	}
	
	//Draw the GUI to the screen
	draw_ignore_frames (obj);
}

void gui_component_hide (gui_component* cpt) {
	
	//Store the old bounds
	rectangle* r = &(cpt->prev_bounds);
	r->x = cpt->ui->x;
	r->y = cpt->ui->y;
	r->width = cpt->ui->width;
	r->height = cpt->ui->height;
	
	//Hide the GUI
	cpt->ui->x = -1;
	cpt->ui->y = -1;
	cpt->ui->width = .001; //To prevent division by zero
	cpt->ui->height = .001;
	
}

void gui_component_show (gui_component* cpt) {
	
	//Show the GUI
	rectangle* r = &(cpt->prev_bounds);
	cpt->ui->x = r->x;
	cpt->ui->y = r->y;
	cpt->ui->width = r->width;
	cpt->ui->height = r->height;
	
}

rectangle* get_layout_elem_by_index (int index) {
}