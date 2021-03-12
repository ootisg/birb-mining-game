#ifndef GEOMETRY_H
#define GEOMETRY_H

///About what you'd expect
struct rectangle {
	float x;
	float y;
	float width;
	float height;
};

typedef struct rectangle rectangle;

///Initializes a rectangle struct at the given pointer with the given attributes.
///@param ptr the location to initialize the rectangle
///@param x the x-coordinate of the rectangle
///@param y the y-coordinate of the rectangle
///@param width the width of the rectangle
///@param height the height of the rectangle
///@return ptr cast to rectangle*
rectangle* make_rectangle (void* ptr, float x, float y, float width, float height);

///Initializes a copy of the given rectangle at the location ptr.
///@param ptr the location to initialize the rectangle
///@param rect the rectangle who's attributes to copy
///@return ptr cast to rectangle*
rectangle* copy_rectangle (void* ptr, rectangle* rect);

///Scales the rectangle to_scale to fit within the rectangle bounds and stores the result at ptr.
///@param ptr the location to put the scaled rectangle
///@param to_scale the rectangle to scale; (0,0) is the top left of bounds, (1, 1) is the bottom right
///@param bounds the rectangle to scale about
///@return ptr cast to rectangle*
rectangle* scale_rectangle (void* ptr, rectangle* to_scale, rectangle* bounds);

///Scales the rectangle (x,y,width,height) to fit within the rectangle bounds and stores the result at ptr.
///@param ptr the location to put the scaled rectangle
///@param to_scale the rectangle to scale; (0,0) is the top left of bounds, (1, 1) is the bottom right
///@param bounds the rectangle to scale about
///@return ptr cast to rectangle*
rectangle* scale_rectangle_from_attributes (void* ptr, rectangle* bounds, float x, float y, float width, float height);

///Checks if rect contains the point (x, y)
///@param rect the rectangle to check
///@param x the x-coordinate of the point
///@param y the y-coordinate of the point
///@return 1 if rect contains (x, y); 0 otherwise
int rectangle_contains (rectangle* rect, float x, float y);

///Checks if the 2 given rectangles overlap, order doesn't matter
///@param r1 a rectangle to check
///@param r2 a rectangle to check
///@return 1 if the rectangles overlap; 0 otherwise
int rectangle_overlaps (rectangle* r1, rectangle* r2);

///Prints the given rectangle to the console
void print_rectangle (rectangle* r);

#endif