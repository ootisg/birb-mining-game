#include "object_handler.h"

object_handler* global_object_handler;

object_handler* make_object_handler (void* ptr) {
	object_handler* ohptr = (object_handler*)ptr;
	ohptr->objects = make_linked_list (malloc (sizeof (linked_list)));
	ohptr->objects_map = make_hash_table (malloc (sizeof (hash_table)));
	return ohptr;
}

int declare_game_object (object_handler* obj_handler, game_object* obj) {
	hash_table* objs_map = obj_handler->objects_map;
	linked_list* typeptr = hash_table_get (objs_map, obj->object_type, strlen (obj->object_type));
	if (typeptr) {
		//Add to the list
		linked_list_add (typeptr, obj, sizeof (game_object));
	} else {
		//List does not exist, add it
		linked_list* new_list = make_linked_list (malloc (sizeof (linked_list)));
		hash_table_put (objs_map, obj->object_type, strlen (obj->object_type), new_list);
		linked_list* type_list = obj_handler->objects;
		linked_list_add (type_list, new_list, sizeof (linked_list));
		//Add the new object to the new list
		linked_list_add (new_list, obj, sizeof (game_object));
	}
	obj->init_call (obj);
}

void run_objs_game_logic (object_handler* obj_handler) {
	//Get the type list
	linked_list* type_list = obj_handler->objects;
	linked_list_iter* iter = linked_list_get_iter (malloc (sizeof (linked_list_iter)), type_list);
	while (linked_list_iter_has_next (iter)) {
		//Iterate through the types
		linked_list_node* curr = linked_list_iter_next (iter);
		linked_list* obj_list = curr->node_data;
		linked_list_iter* obj_iter = linked_list_get_iter (malloc (sizeof (linked_list_iter)), obj_list);
		while (linked_list_iter_has_next (obj_iter)) {
			//Iterate through the objects of each type
			linked_list_node* curr_obj = linked_list_iter_next (obj_iter);
			game_object* obj = curr_obj->node_data;
			((obj->game_logic_call))(obj);
		}
	}
}

void run_objs_draw (object_handler* obj_handler) {
	//Get the type list
	linked_list* type_list = obj_handler->objects;
	linked_list_iter* iter = linked_list_get_iter (malloc (sizeof (linked_list_iter)), type_list);
	while (linked_list_iter_has_next (iter)) {
		//Iterate through the types
		linked_list_node* curr = linked_list_iter_next (iter);
		linked_list* obj_list = curr->node_data;
		linked_list_iter* obj_iter = linked_list_get_iter (malloc (sizeof (linked_list_iter)), obj_list);
		while (linked_list_iter_has_next (obj_iter)) {
			//Iterate through the objects of each type
			linked_list_node* curr_obj = linked_list_iter_next (obj_iter);
			game_object* obj = curr_obj->node_data;
			((obj->draw_call))(obj);
		}
	}
}

object_handler* get_global_object_handler () {
	if (!global_object_handler) {
		global_object_handler = make_object_handler (malloc (sizeof (object_handler)));
	}
	return global_object_handler;
}

linked_list* get_objs_by_type (object_handler* obj_handler, char* type) {
	return hash_table_get (obj_handler->objects_map, type, strlen (type));
}

int game_object_colliding_type (object_handler* obj_handler, game_object* obj, char* type) {
	linked_list* obj_list = get_objs_by_type (obj_handler, type);
	linked_list_node* curr = obj_list->head;
	while (curr) {
		if (curr->node_data != obj && obj->is_colliding (obj, curr->node_data)) {
			return 1;
		}
		curr = curr->next;
	}
	return 0;
}

linked_list* game_object_get_colliding (object_handler* obj_handler, game_object* obj, char* type) {
	linked_list* colliding_list = make_linked_list (malloc (sizeof (linked_list)));
	linked_list* obj_list = get_objs_by_type (obj_handler, type);
	linked_list_node* curr = obj_list->head;
	while (curr) {
		if (curr->node_data != obj && obj->is_colliding (obj, curr->node_data)) {
			linked_list_add (colliding_list, curr->node_data, sizeof (game_object));
		}
		curr = curr->next;
	}
	return colliding_list;
}