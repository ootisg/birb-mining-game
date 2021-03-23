#include "birb.h"

#include "inputs.h"
#include "game.h"

sprite* birb_sprite = NULL;

void init_birb (game_object* obj) {
	
	//Run the default init
	default_init (obj);
	
	//Set the birb depth
	obj->depth = -1.0f;
	
	//Load the birb sprite if it's not loaded
	if (!birb_sprite) {
		birb_sprite = make_sprite_from_json ("resources/sprites/config/birb.json", NULL);
	}
	
	//Set the birb sprite and animation speed
	obj->sprite = birb_sprite;
	obj->animator.frame_time = 250;
	
}

void birb_logic (game_object* obj) {
	
	//Get the inputs
	input_state* inputs = get_inputs ();
	
	//Do key checks
	if (inputs->keys_down['w']) {
		obj->y -= .01;
	}
	if (inputs->keys_down['a']) {
		obj->x -= .01;
	}
	if (inputs->keys_down['s']) {
		obj->y += .01;
	}
	if (inputs->keys_down['d']) {
		obj->x += .01;
	}
	
}

game_object* make_birb () {
	
	//Allocate the birb
	game_object* birb = make_game_object (malloc (sizeof (game_object)), "birb");
	
	//Setup the birb's callbacks
	birb->init_call = init_birb;
	birb->game_logic_call = birb_logic;
	birb->draw_call = viewport_draw;
	
	//Declare the birb
	declare_game_object (get_global_object_handler (), birb);
	
	//Return the birb
	return birb;
	
}

