#ifndef OBJECT_HANDLER_H
#define OBJECT_HANDLER_H

#include <stdlib.h>

#include "linked_list.h"
#include "hash_table.h"
#include "game_object.h"

///Struct for handling/tracking game_objects
struct object_handler {
	linked_list* objects;
	hash_table* objects_map;
};

///nice
typedef struct object_handler object_handler;

///Gets the global object handler, because putting this in main.h creates a circular dependancy
object_handler* get_global_object_handler ();

///Initializes an object_handler struct at the given pointer and returns it.
///@param ptr the location to initialize the object_handler
///@return ptr cast to object_handler*
object_handler* make_object_handler (void* ptr);

///Declares the game_obj obj to the given obj_handler. This allows obj_handler to track obj. This also calls obj->init_call (obj).
///@param obj_handler the object_handler to declare to
///@param obj the object to declare
///@return always returns 0
int declare_game_object (object_handler* obj_handler, game_object* obj);

///Runs game_logic_call for every game_object tracked by obj_handler.
///@param obj_handler the object_handler to use
void run_objs_game_logic (object_handler* obj_handler);

///Runs draw_call for every game_object tracked by obj_handler.
///@param obj_handler the object_handler to use
void run_objs_draw (object_handler* obj_handler);

///Gets all objects of the given type.
///@param obj_handler the object_handler to search
///@param type the type string to search for
///@return the list of all objects of the given type; null if none are found
linked_list* get_objs_by_type (object_handler* obj_handler, char* type);

///Checks if obj is colliding with an object of the given type tracked by obj_handler.
///@param obj_handler the object_handler to search
///@param obj the game_object to do collision checks against
///@param type the game_object type to search for
///@return 1 if a collision was detected; 0 otherwise
int game_object_colliding_type (object_handler* obj_handler, game_object* obj, char* type);

///Gets the list of all game_objects tracked by obj_handler of the given type which are colliding with obj.
///@param obj_handler the object_handler to search
///@param obj the game_object to do collision checks against
///@param type the game_object type to search for
///@return a linked_list of all colliding objects of the given type. Can be free()'d.
linked_list* game_object_get_colliding (object_handler* obj_handler, game_object* obj, char* type);

#endif