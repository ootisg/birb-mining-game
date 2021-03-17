#ifndef GAME_H
#define GAME_H

/// Runs once at the beginning of the game to initialize everything which needs initialized
void game_init ();

/// Runs once per frame, before all game_object calls
void game_logic_loop ();

#endif