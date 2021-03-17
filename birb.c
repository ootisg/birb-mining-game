#include "birb.h"

sprite* birb_sprite = NULL;

void init_birb () {
	if (!birb_sprite) {
		birb_sprite = make_sprite_from_json ("resources/sprites/config/birb.json", NULL);
	}
}

game_object* make_birb () {
	
	game_object* birb = make_game_object (malloc (sizeof (game_object)), "birb");
	declare_game_object (get_global_object_handler (), birb);
	birb->sprite = birb_sprite;
	birb->animator.frame_time = 250;
	return birb;
	
}