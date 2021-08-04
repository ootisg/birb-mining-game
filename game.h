#ifndef GAME_H
#define GAME_H

#include "game_object.h"

/// Runs once at the beginning of the game to initialize everything which needs initialized
void game_init ();

/// Runs once per frame, before all game_object calls
void game_logic_loop ();

/// Default draw function for drawing inside a viewport
/// @param obj the game_object being drawn
void viewport_draw (game_object* obj);

//Toggles the opened menu
void toggle_menu ();

//Opens the shop menu
void open_shop ();

#endif