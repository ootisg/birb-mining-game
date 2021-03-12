#ifndef LAYOUT_H
#define LAYOUT_H

#include "json.h"
#include "linked_list.h"
#include "geometry.h"

//Documentation will happen soon

struct layout_element {
	rectangle bounds;
	linked_list children;
};

typedef struct layout_element layout_element;

layout_element* make_layout (layout_element* ptr, json_object* description);
linked_list* get_layout_reigons (layout_element* layout, rectangle* bounds);
void free_layout (layout_element* layout);

#endif