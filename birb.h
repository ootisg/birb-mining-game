#ifndef BIRB_H
#define BIRB_H

#include "game_object.h"
#include "sprite.h"
#include "object_handler.h"

#define BIRB_MOVE_TIME 250

/// Initializes the resources required for le birb
void init_birb ();

/// Makes a new birb
/// @return a fresh, clean birb ready for adventure
game_object* make_birb ();

/// Makes the break animation for the birb, this doesn't need to be in the header
/// @return the break animation
game_object* make_break_anim ();

/// Reveals the area around the given birb
/// @param obj the birb to reveal around
void reveal_around_birb (game_object* obj);

#endif