#ifndef STACK_H
#define STACK_H

#include <stdlib.h>

#include "char_buffer.h"

/// A struct representing a stack implemented by an expanding buffer
struct stack {
	char_buffer* data;
	int element_size;
	void* stack_ptr;
};

typedef struct stack stack;

/// Creates a new stack struct with the following initial values:
///		data: a default char_buffer created by calling make_char_buffer ();
///		element_size: initialized to the argument element_size
///		stack_ptr: a pointer to the top of the stack. Initialized to data->data (hmm, maybe data is a bad name for a field)
///	@param element_size the element size to store in the stack, in bytes; each element on the stack must be smaller than this field
/// @return the newly created stack
stack* init_stack (int element_size);

/// Pushes the given element onto the given stack
/// @param s the stack to push to
/// @param element the element to push onto the stack
/// @return non-zero if the location of the stack changed when the element was pushed, 0 otherwise
int push (stack* s, void* element);

/// Pops an element from the given stack
/// @param s the stack to pop from
/// @return the popped element
void* pop (stack* s);

/// Returns the number of elements on the given stack
/// @param s the stack to query
/// @return the number of elements in s
int stack_size (stack* s);

#endif