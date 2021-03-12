#include <stdio.h>

#include "geometry.h"

rectangle* make_rectangle (void* ptr, float x, float y, float width, float height) {
	rectangle* r_ptr = (rectangle*)ptr;
	r_ptr->x = x;
	r_ptr->y = y;
	r_ptr->width = width;
	r_ptr->height = height;
	return r_ptr;
}

rectangle* copy_rectangle (void* ptr, rectangle* r) {
	rectangle* r_ptr = (rectangle*)ptr;
	r_ptr->x = r->x;
	r_ptr->y = r->y;
	r_ptr->width = r->width;
	r_ptr->height = r->height;
	return r_ptr;
}

rectangle* scale_rectangle (void* ptr, rectangle* to_scale, rectangle* bounds) {
	rectangle* r_ptr = (rectangle*)ptr;
	r_ptr->x = bounds->x + to_scale->x * bounds->width;
	r_ptr->y = bounds->y + to_scale->y * bounds->height;
	r_ptr->width = bounds->width * to_scale->width;
	r_ptr->height = bounds->height * to_scale->height;
	return r_ptr;
}

rectangle* scale_rectangle_from_attributes (void* ptr, rectangle* bounds, float x, float y, float width, float height) {
	rectangle* r_ptr = (rectangle*)ptr;
	r_ptr->x = bounds->x + x * bounds->width;
	r_ptr->y = bounds->y + y * bounds->height;
	r_ptr->width = bounds->width * width;
	r_ptr->height = bounds->height * height;
	return r_ptr;
}

void print_rectangle (rectangle* r) {
	printf ("RECTANGLE: [x: %f, y: %f, width: %f, height: %f]\n", r->x, r->y, r->width, r->height);
}

int rectangle_contains (rectangle* rect, float x, float y) {
	if (x >= rect->x && x < (rect->x + rect->width) && y >= rect->y && y < (rect->y + rect->height)) {
		return 1; //True
	}
	return 0; //False
}

int rectangle_overlaps (rectangle* r1, rectangle* r2) {
	//This is witchcraft
	if (r1->x > r2->x - r1->width && r1->x < r2->x + r2->width && r1->y > r2->y - r1->height && r1->y < r2->y + r2->height) {
		return 1;
	}
	return 0;
}