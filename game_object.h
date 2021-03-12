#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include <stdlib.h>

#include <GL/glew.h>
#include <GL/glut.h>

#include "main.h"
#include "geometry.h"

#define ANIMATION_HANDLER_TYPE_FRAME 0x0
#define ANIMATION_HANDLER_TYPE_MS 0x1
#define ANIMATION_HANDLER_TYPE 0x1

#define ANIMATION_HANDLER_LOOP_STOP 0x0
#define ANIMATION_HANDLER_LOOP_REPEAT 0x2
#define ANIMATION_HANDLER_LOOP 0x2

#define ANIMATION_HANDLER_FINISHED 0x4

struct animation_handler {
	int flags;
	int start_time;
	int frame_count;
	float frame_time;
};

/// A struct representing a game object (also woefully underdocumented)
struct game_object {
	char* object_type;
	void* object_data;
	GLfloat* vertex_data;
	void (*init_call)(struct game_object*);
	void (*draw_call)(struct game_object*);
	void (*game_logic_call)(struct game_object*);
	int (*is_colliding)(struct game_object*, struct game_object*);
	sprite* sprite;
	struct animation_handler animator;
	double x;
	double y;
	double width;
	double height;
	rectangle hitbox;
};

typedef struct game_object game_object;
typedef struct animation_handler animation_handler;

/// Makes a new game_object at the given location
/// @param ptr the location to make the game_object at
/// @param type the type name of the game_object
/// @return ptr cast to a game_object*
game_object* make_game_object (void* ptr, char* type);

/// Initializes a new animation_handler at the location ptr
/// Default properties: animates 1 frame, looping, using ms at 60fps (17 ms/frame)
/// @return ptr cast to animation_handler*
animation_handler* make_animation_handler (void* ptr);

/// Sets the properties defined by the bitmask properties
/// 	0x1: Timing type, 0 for timing by frame, 1 for timing by ms
///		0x2: Loop, 0 for no loop, 1 for looping animations
///		0x4: Finished, 0 when not finished, 1 otherwise; only applicable to non-looping animations
///	@param ptr the animation_handler to use
/// @param properties a bitmask of the properties to set/clear
/// @param val sets the specified properties if non-zero; clears them otherwise
void animation_handler_set_properties (animation_handler* ptr, int properties, int val);

/// Gets the properties defined by the following bitmask:
/// 	0x1: Timing type, 0 for timing by frame, 1 for timing by ms
///		0x2: Loop, 0 for no loop, 1 for looping animations
///		0x4: Finished, 0 when not finished, 1 otherwise; only applicable to non-looping animations
///	@param ptr the animation_handler to use
/// @param properties a bitmask of the properties to query
/// @return zero if all the specified properties are 0; non-zero otherwise
int animation_handler_get_properties (animation_handler* ptr, int property);

/// Gets the current animation frame of the given animation_handler ptr
/// @param the animation_handler to use
/// @return the frame of animation the animation_handler ptr is on
int animation_handler_get_frame (animation_handler* ptr);

/// The default init function for a game_object
/// @param obj the game_object running this function
void default_init (game_object* obj);

/// The default game logic function for a game_object
/// @param obj the game_object running this function
void default_game_logic (game_object* obj);

/// The default draw function for a game_object
/// @param obj the game_object running this function
void default_draw (game_object* obj);

/// The default hitbox generation function for game_object, which uses its x, y, width, and height
/// @param obj the game_object whose hitbox will be initialized
void generate_hitbox (game_object* obj);

#endif