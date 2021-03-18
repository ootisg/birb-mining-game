#ifndef VIEWPORT_H
#define VIEWPORT_H

#include "geometry.h"

/// Transforms the given point from map coordinates to viewport coordinates
/// @param viewport the viewport to use
/// @param pt the point to transform
void transform_to_viewport (rectangle* viewport, point* pt);

/// Gets the current viewport
/// @return a bounding rectangle representing the current viewport
rectangle* get_viewport ();

/// Updates the current viewport based on user input
void update_viewport ();

#endif