#include "stack.h"

stack* init_stack (int element_size) {
	stack* s = malloc (sizeof (stack));
	s->data = make_char_buffer ();
	s->element_size = element_size;
	s->stack_ptr = (void*)(s->data);
}

int push (stack* s, void* element) {
	
	//Store the old buffer pointer
	char* buffer_ptr = s->data->data;
	
	//Add to the stack and update the stack pointer
	char_buffer_add_item (s->data, element, s->element_size);
	s->stack_ptr = (void*)(s->data->data + s->data->length);
	
	//Check if the buffer was resized (moved)
	return buffer_ptr != s->data->data;
}

void* pop (stack* s) {
	return s->stack_ptr = (void*)(((char*)(s->stack_ptr)) - s->element_size);
}

int stack_size (stack* s) {
	return s->data->length / s->element_size;
}