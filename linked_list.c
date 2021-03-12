#include "linked_list.h"

linked_list* make_linked_list (void* ptr) {
	linked_list* list_ptr = (linked_list*)ptr;
	list_ptr->head = (linked_list_node*)0;
	list_ptr->tail = (linked_list_node*)0;
	list_ptr->size = 0;
	return list_ptr;
}

linked_list_node* make_linked_list_node (void* node_data, int node_data_size) {
	linked_list_node* node_ptr = malloc (sizeof (linked_list_node));
	node_ptr->next = (linked_list_node*)0;
	node_ptr->prev = (linked_list_node*)0;
	node_ptr->node_data = node_data;
	node_ptr->node_data_size = node_data_size;
	return node_ptr;
}

int linked_list_add_front (linked_list* list_ptr, void* element, int element_size) {
	linked_list_node* new_node = make_linked_list_node (element, element_size);
	if (list_ptr->size == 0) {
		//Empty list case
		list_ptr->head = new_node;
		list_ptr->tail = new_node;
		list_ptr->size = 1;
		return list_ptr->size;
	} else {
		//Add element to the rest of the list
		new_node->next = list_ptr->head;
		list_ptr->head->prev = new_node;
		list_ptr->head = new_node;
		list_ptr->size++;
		return list_ptr->size;
	}
}

int linked_list_add (linked_list* list_ptr, void* element, int element_size) {
	linked_list_node* new_node = make_linked_list_node (element, element_size);
	if (list_ptr->size == 0) {
		//Empty list case
		list_ptr->head = new_node;
		list_ptr->tail = new_node;
		list_ptr->size = 1;
		return list_ptr->size;
	} else {
		//Add element to the rest of the list
		new_node->prev = list_ptr->tail;
		list_ptr->tail->next = new_node;
		list_ptr->tail = new_node;
		list_ptr->size++;
		return list_ptr->size;
	}
}

linked_list_iter* linked_list_get_iter (void* ptr, linked_list* list_ptr) {
	linked_list_iter* iter_ptr = (linked_list_iter*)ptr;
	iter_ptr->list = list_ptr;
	iter_ptr->current_node = list_ptr->head;
	iter_ptr->last_node = (linked_list_node*)0;
	return iter_ptr;
}

int linked_list_iter_has_next (linked_list_iter* iter) {
	return (int)(iter->current_node != 0);
}

linked_list_node* linked_list_iter_next (linked_list_iter* iter) {
	iter->last_node = iter->current_node;
	iter->current_node = iter->current_node->next;
	return iter->last_node;
}

linked_list_node* linked_list_iter_add (linked_list_iter* iter, void* element, int element_size) {
	
	//Get the relevant nodes
	linked_list_node* new_node = make_linked_list_node (element, element_size);
	linked_list_node* cur = iter->current_node;
	
	//Add the node to the list
	if (iter->list->size == 0) {
		//Empty list case
		iter->list->head = new_node;
		iter->list->tail = new_node;
	} else if (!cur) {
		//End of the list case
		new_node->prev = iter->list->tail;
		iter->list->tail->next = new_node;
		iter->list->tail = new_node;
	} else {
		if (cur->prev) {
			//Link the previous node
			cur->prev->next = new_node;
			new_node->prev = cur->prev;
		} else {
			//Start of the list case
			iter->list->head = new_node;
		}
		//Link the next node
		cur->prev = new_node;
		new_node->next = cur;
	}
	iter->list->size++;
	return new_node;
}

linked_list_node* linked_list_iter_remove (linked_list_iter* iter) {
	
	linked_list_node* to_remove = iter->last_node;
	if (to_remove->prev) {
		//Update previous node
		to_remove->prev->next = to_remove->next;
	} else {
		//Start of the list case
		iter->list->head = to_remove->next;
	}
	if (to_remove->next) {
		//Update next node
		to_remove->next->prev = to_remove->prev;
	} else {
		//End of the list case
		iter->list->tail = to_remove->prev;
	}
	iter->list->size--;
	return to_remove;
}

void free_linked_list_elements (linked_list* list, void* data_callback) {
	
	//Initialize important values
	linked_list_node* cur = list->head;
	linked_list_node* prev = (linked_list_node*)0;
	void (*foo)(void*);
	foo = data_callback;
	
	//Iterate through the list and free all the nodes, data
	while (cur) {
		prev = cur;
		cur = cur->next;
		if (data_callback) {
			//Free the data stored in this node
			foo (prev->node_data);
		}
		free (prev);
	}
	//Free the last element of the list (unsure if this ever runs, TODO check on that)
	if (prev) {
		if (data_callback) {
			//Free the data stored in this node
			foo (prev->node_data);
		}
		free (prev);
	}
}