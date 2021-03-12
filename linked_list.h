#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <stdlib.h>

/// A struct representing a node in a linked_list
struct linked_list_node {
	struct linked_list_node* next;
	struct linked_list_node* prev;
	void* node_data;
	int node_data_size;
};

/// A struct representing a linked list
struct linked_list {
	struct linked_list_node* head;
	struct linked_list_node* tail;
	int size;
};

/// A struct representing a linked list iterator
struct linked_list_iter {
	struct linked_list* list;
	struct linked_list_node* current_node;
	struct linked_list_node* last_node;
};

typedef struct linked_list linked_list;
typedef struct linked_list_node linked_list_node;
typedef struct linked_list_iter linked_list_iter;

/// Initializes a linked_list struct at the given location. Initial values are as follows:
/// 	head: initialized to null
///		tail: initialized to null
///		size: initialized to 0
/// @param ptr the location to initialize the linked_list at
/// @return ptr cast to a linked_list*
linked_list* make_linked_list (void* ptr);

/// Creates a linked_list_node struct and returns its location. Initial values are as follows:
///		next: initialized to null
///		prev: initialized to null
///		node_data: set to the argument node_data
///		node_data_size: set to the argument node_data_size
/// @param node_data the data to store in this node
/// @param node_data_size the size of the data to store, in bytes
/// @return the newly created linked_list_node
linked_list_node* make_linked_list_node (void* node_data, int node_data_size);

/// Adds an element to the end of a linked_list.
/// @param list_ptr the list to add to
/// @param element the element to add
/// @param element_size the size of the element to add, in bytes
/// @return the new size of the list
int linked_list_add (linked_list* list_ptr, void* element, int element_size);

/// Adds an element to the front of a linked_list.
/// @param list_ptr the list to add to
/// @param element the element to add
/// @param element_size the size of the element to add, in bytes
/// @return the new size of the list
int linked_list_add_front (linked_list* list_ptr, void* element, int element_size);

/// Initializes a linked_list_iter for the given linked_list at the given location.
/// @param ptr the location to initialize the linked_list_iter at
/// @param list_ptr the linked_list to make an iter for
/// @return ptr cast to a linked_list_iter*
linked_list_iter* linked_list_get_iter (void* ptr, linked_list* list_ptr);

/// Checks if the given linked_list_iter has a next element
/// @param iter the iter to check
/// @return a non-zero value if iter has a next element; 0 otherwise
int linked_list_iter_has_next (linked_list_iter* iter);

/// Advances the given linked_list_iter by one element and returns it.
/// @param iter the linked_list_iter to advance
/// @return the element that was iterated over
linked_list_node* linked_list_iter_next (linked_list_iter* iter);

/// Adds an element after the last iterated element of the given linked_list_iter, to its associated linked list. Subsequent calls to linked_list_iter_next are unaffected.
/// 	Example: list is [0, 1, 3, 5]
/// 	After calling linked_list_iter_next twice (returning 0 and 1 respectively), linked_list_iter_add is called to add the element 2
///		The list is now [0, 1, 2, 3, 5]. The next call to linked_list_next will return 3.
///		Calling this with a newly created linked_list_iter will add to the start of the list; a linked_list_iter with no remaining elements will add to the end of the list
/// @param iter the linked_list_iter to use
/// @param element the element to add
/// @param elemet_size the size of the element to add, in bytes
/// @return the node that was added
linked_list_node* linked_list_iter_add (linked_list_iter* iter, void* element, int element_size);

/// Removes the last iterated element of the given linked_list_iter, from its associated linked list. Subsequent calls to linked_list_iter_next are unaffected.
/// 	Example: list is [0, 1, 3, 5]
///		After calling linked_list_iter_next twice (returning 0 and 1 respectively), linked_list_iter_remove is called
///		The list is now [0, 3, 5]. The next call to linked_list_next will return 3.
///		Calling this with a newly created linked_list_iter will remove the first element of the list; a linked_list_iter with no remaining elements will remove the last element of the list.
///	@param iter the linked_list_iter to use
/// @return the removed node
linked_list_node* linked_list_iter_remove (linked_list_iter* iter);

/// Frees all the elements of the given linked list
///	@param list the list to use
/// @param data_callback function which returns void and takes a single void*, to be called on every element stored in the list; set to null to specify 'don't use'
void free_linked_list_elements (linked_list* list, void* data_callback);

#endif