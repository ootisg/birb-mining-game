#include "layout.h"

layout_element* make_layout_rec (layout_element* ptr, json_object* description) {
	//oop
	rectangle* parent_bounds = &(ptr->bounds);
	//Make the children list
	linked_list* children_list = make_linked_list (&(ptr->children));
	//Get the list of properties
	json_object* json_obj = json_get_object (description, "layout");
	if (json_obj) {
		linked_list* json_elems = (linked_list*)(json_obj->token_data);
		linked_list_node* curr = json_elems->head;
		while (curr) {
			json_object* curr_obj = curr->node_data;
			char* type_str = json_get_string (curr_obj, "type");
			if (!strcmp (type_str, "rect")) {
				//Rectangle
				float new_x = *((float*)(json_get_object (curr_obj, "x")->token_data));
				float new_y = *((float*)(json_get_object (curr_obj, "y")->token_data));
				float new_width = *((float*)(json_get_object (curr_obj, "width")->token_data));
				float new_height = *((float*)(json_get_object (curr_obj, "height")->token_data));
				layout_element* new_layout = malloc (sizeof (layout_element));
				scale_rectangle_from_attributes (&(new_layout->bounds), parent_bounds, new_x, new_y, new_width, new_height);
				linked_list_add (children_list, new_layout, sizeof (layout_element));
				make_layout_rec (new_layout, curr_obj);
			} else if (!strcmp (type_str, "grid")) {
				//Grid
				float new_width = *((float*)(json_get_object (curr_obj, "width")->token_data));
				float new_height = *((float*)(json_get_object (curr_obj, "height")->token_data));
				for (float wy = parent_bounds->y; wy + new_height <= parent_bounds->y + parent_bounds->height + .00001; wy += new_height) {
					for (float wx = parent_bounds->x; wx + new_width <= parent_bounds->x + parent_bounds->width + .00001; wx += new_width) {
						//TODO find a better way to deal with floating point weirdnes
						layout_element* new_layout = malloc (sizeof (layout_element));
						scale_rectangle_from_attributes (&(new_layout->bounds), parent_bounds, wx, wy, new_width, new_height);
						print_rectangle (&(new_layout->bounds));
						linked_list_add (children_list, new_layout, sizeof (layout_element));
						make_layout_rec (new_layout, curr_obj);
					}
				}
			}
			make_layout (malloc (sizeof (layout_element)), curr->node_data);
			curr = curr->next;
		}
	}
	return ptr;
}

layout_element* make_layout (layout_element* ptr, json_object* description) {
	make_rectangle (&(ptr->bounds), 0.0, 0.0, 1.0, 1.0);
	return make_layout_rec (ptr, description);
}

void add_layout_reigons (layout_element* layout, linked_list* layout_list, rectangle* bounds) {
	linked_list* to_add = &(layout->children);
	linked_list_node* curr = to_add->head;
	if (!curr) {
		rectangle* new_rect;
		if (bounds) {
			new_rect = scale_rectangle (malloc (sizeof (rectangle)), (&(layout->bounds)), bounds);
		} else {
			new_rect = copy_rectangle (malloc (sizeof (rectangle)), (&(layout->bounds)));
		}
		linked_list_add (layout_list, new_rect, sizeof (rectangle));
		return;
	}
	while (curr) {
		add_layout_reigons (curr->node_data, layout_list, bounds);
		curr = curr->next;
	}
}

linked_list* get_layout_reigons (layout_element* layout, rectangle* bounds) {
	linked_list* reigons_list = make_linked_list (malloc (sizeof (linked_list)));
	add_layout_reigons (layout, reigons_list, bounds);
	return reigons_list;
}

void free_layout (layout_element* layout) {
	linked_list* to_free = &(layout->children);
	linked_list_node* curr = to_free->head;
	while (curr) {
		free_layout (curr->node_data);
		curr = curr->next;
	}
	free (layout);
}