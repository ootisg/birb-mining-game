#include "birb.h"

#include "inputs.h"
#include "game.h"
#include "tile_map.h"
#include "viewport.h"

#define BREAK_ANIM_FRAMES 4

sprite* birb_sprite = NULL;
sprite* anim_sprite = NULL;

struct birb_data {
	int move_time;
	int move_start;
	float orig_x;
	float orig_y;
	float move_x;
	float move_y;
	char key_cached;
	hash_table* drill;
	float drill_strength;
	rectangle* saved_viewport;
	game_object* brk_anim;
};

struct brk_anim_data {
	void* tile;
	float drill_progress;
};

typedef struct birb_data birb_data;
typedef struct brk_anim_data brk_anim_data;

int birb_cam = 1;
int shake_timer = -1;

void init_birb (game_object* obj) {

	//Init the sprite resources if they aren't loaded
	if (!birb_sprite) {
		birb_sprite = make_sprite_from_json ("resources/sprites/config/birb.json", NULL);
	}
	if (!anim_sprite) {
		anim_sprite = make_sprite_from_json ("resources/sprites/config/break.json", NULL);
	}

	//Run the default init
	default_init (obj);
	
	//Set the birb depth
	obj->depth = -1.0f;
	
	//Set the birb's width and height, starting position
	obj->width = MAP_TILE_SIZE;
	obj->height = MAP_TILE_SIZE;
	obj->x = .5;
	obj->y = .8125;
	
	//Setup the birb data
	birb_data* obj_data = malloc (sizeof (birb_data));
	obj->object_data = obj_data;
	obj_data->move_time = -1;
	obj_data->move_start = -1;
	obj_data->orig_x = obj->x;
	obj_data->orig_y = obj->y;
	obj_data->move_x = 0;
	obj_data->move_y;
	obj_data->key_cached = 0;
	obj_data->saved_viewport = NULL;
	//Setup the drill
	obj_data->drill = make_hash_table (malloc (sizeof (hash_table)));
	hash_table_put (obj_data->drill, "soil", 4, &(obj_data->drill));
	obj_data->drill_strength = 0.2f;
	//Setup the break animation
	obj_data->brk_anim = make_break_anim ();
	
	//Set the birb sprite and animation speed
	obj->sprite = birb_sprite;
	animation_handler_set_properties (&(obj->animator), ANIMATION_HANDLER_STILL_FRAME, 1);
	
}

void break_tile (game_object* obj, int x, int y) {
	
	//Get some important data
	birb_data* obj_data = obj->object_data;
	brk_anim_data* brk_data = obj_data->brk_anim->object_data;
	map_tile* t = map_get_tile (x, y);
	char* material = get_tile_property (t->id, "material");
	
	//Do the block mining
	if (t->tile_obj) {
		if (hash_table_get (obj_data->drill, material, strlen (material))) {
			//Block can be mined
			if (brk_data->tile != t) {
				//Mining a different tile, swap to the new one
				obj_data->brk_anim->x = t->tile_obj->x;
				obj_data->brk_anim->y = t->tile_obj->y;
				animation_handler_set_frame (&(obj_data->brk_anim->animator), 0);
				brk_data->tile = t;
				brk_data->drill_progress = 0.0f;
			} else {
				//Continue mining the current tile
				brk_data->drill_progress += obj_data->drill_strength;
				int hardness = *((int*)get_tile_property (t->id, "hardness"));
				int frame = (brk_data->drill_progress / hardness) * 4;
				if (frame < BREAK_ANIM_FRAMES) {
					//Update the animation
					animation_handler_set_frame (&(obj_data->brk_anim->animator), frame);
				} else {
					//Destroy the tile
					t->id = rand () > RAND_MAX / 2 ? tile_id_by_name ("bg_1") : tile_id_by_name ("bg_2");
					obj_data->brk_anim->x = -1;
					obj_data->brk_anim->y = -1;
					brk_data->tile = NULL;
				}
			}
		} else {
			shake_timer = 12;
		}
	} else {
		//Force the camera on-screen
		get_viewport ()->x = obj->x - .5;
		get_viewport ()->y = obj->y - .5;
		bind_viewport ();
	}
	
	force_update_tile (t);
}

void move_to (game_object* obj, float x, float y, float frames) {

	//Check for OOB
	if (x < 0 || y < 0 || x >= MAP_GRID_WIDTH * MAP_TILE_SIZE || y >= MAP_GRID_HEIGHT * MAP_TILE_SIZE) {
		return; //Cancel the move
	}

	//Check for tiles
	int tile_x = x / MAP_TILE_SIZE;
	int tile_y = y / MAP_TILE_SIZE;
	map_tile* t = map_get_tile (tile_x, tile_y);
	char* tile_type = get_tile_property (t->id, "type");
	if (strcmp (tile_type, "bg")) {
		break_tile (obj, tile_x, tile_y);
		return; //Cancel the move
	}
	
	//Tell the birb to move
	birb_data* obj_data = obj->object_data;
	obj_data->orig_x = obj->x;
	obj_data->orig_y = obj->y;
	obj_data->move_x = x;
	obj_data->move_y = y;
	obj_data->move_time = frames;
	obj_data->move_start = get_frame_time_ms ();
	
}

int do_move (game_object* obj) {
	birb_cam = 1;
	birb_data* obj_data = obj->object_data;
	float progress = (float)(get_frame_time_ms () - obj_data->move_start) / obj_data->move_time;
	if (progress >= 1) {
		return 1;
	}
	obj->x = obj_data->orig_x + (obj_data->move_x - obj_data->orig_x) * progress;
	obj->y = obj_data->orig_y + (obj_data->move_y - obj_data->orig_y) * progress;
	return 0;
}

void birb_logic (game_object* obj) {
	
	//Get the birb data
	birb_data* obj_data = obj->object_data;
	
	//Mess with the camera
	if (birb_cam) {
		get_viewport ()->x = obj->x - .5;
		get_viewport ()->y = obj->y - .5;
		bind_viewport ();
	}
	
	//Update the camera shaky
	if (shake_timer >= 0) {
		if (!obj_data->saved_viewport) {
			obj_data->saved_viewport = malloc (sizeof (rectangle));
			obj_data->saved_viewport->x = get_viewport ()->x;
			obj_data->saved_viewport->y = get_viewport ()->y;
			obj_data->saved_viewport->width = get_viewport ()->width;
			obj_data->saved_viewport->height = get_viewport ()->height;
		}
		rectangle* saved_viewport = obj_data->saved_viewport;
		float offs_x = (float)rand () / RAND_MAX * .01;
		float offs_y = (float)rand () / RAND_MAX * .01;
		make_rectangle (get_viewport (), saved_viewport->x + offs_x, saved_viewport->y + offs_y, saved_viewport->width, saved_viewport->height);
		shake_timer--;
	} else if (obj_data->saved_viewport) {
		make_rectangle (get_viewport (), obj_data->saved_viewport->x, obj_data->saved_viewport->y, obj_data->saved_viewport->width, obj_data->saved_viewport->height);
		free (obj_data->saved_viewport);
		obj_data->saved_viewport = NULL;
	}
	
	//Get the inputs
	input_state* inputs = get_inputs ();
	
	//Do key checks
	if (obj_data->move_start == -1) {
		if (inputs->keys_down['w'] || obj_data->key_cached == 'w') {
			move_to (obj, obj->x, obj->y - MAP_TILE_SIZE, BIRB_MOVE_TIME);
			animation_handler_set_frame (&(obj->animator), 1);
			obj_data->key_cached = 0;
		}
		if (inputs->keys_down['a'] || obj_data->key_cached == 'a') {
			move_to (obj, obj->x - MAP_TILE_SIZE, obj->y, BIRB_MOVE_TIME);
			animation_handler_set_frame (&(obj->animator), 0);
			obj_data->key_cached = 0;
		}
		if (inputs->keys_down['s'] || obj_data->key_cached == 's') {
			move_to (obj, obj->x, obj->y + MAP_TILE_SIZE, BIRB_MOVE_TIME);
			animation_handler_set_frame (&(obj->animator), 3);
			obj_data->key_cached = 0;
		}
		if (inputs->keys_down['d'] || obj_data->key_cached == 'd') {
			move_to (obj, obj->x + MAP_TILE_SIZE, obj->y, BIRB_MOVE_TIME);
			animation_handler_set_frame (&(obj->animator), 2);
			obj_data->key_cached = 0;
		}
	} else {
		if (inputs->keys_pressed['w']) {
			obj_data->key_cached = obj_data->key_cached == 's' ? 0 : 'w';
		}
		if (inputs->keys_pressed['a']) {
			obj_data->key_cached = obj_data->key_cached == 'd' ? 0 : 'a';
		}
		if (inputs->keys_pressed['s']) {
			obj_data->key_cached = obj_data->key_cached == 'w' ? 0 : 's';
		}
		if (inputs->keys_pressed['d']) {
			obj_data->key_cached = obj_data->key_cached == 'a' ? 0 : 'd';
		}
		if (do_move (obj)) {
			obj->x = obj_data->move_x;
			obj->y = obj_data->move_y;
			obj_data->move_time = -1;
			obj_data->move_start = -1;
			birb_cam = 0;
		}
	}
	
	update_viewport ();
	update_tile_objs ();
	
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

game_object* make_break_anim () {
	
	//Allocate the brk_anim
	game_object* brk_anim = make_game_object (malloc (sizeof (game_object)), "break_animation");
	
	//Setup the brk_anim's callbacks
	brk_anim->init_call = default_init;
	brk_anim->game_logic_call = default_game_logic;
	brk_anim->draw_call = viewport_draw;
	
	//Declare the brk_anim
	declare_game_object (get_global_object_handler (), brk_anim);
	
	//Setup the break animation's sprite
	brk_anim->sprite = anim_sprite;
	
	//Setup the break animation's size and position
	brk_anim->x = -1;
	brk_anim->y = -1;
	brk_anim->width = MAP_TILE_SIZE;
	brk_anim->height = MAP_TILE_SIZE;
	
	//Setup the break animation's additional data
	brk_anim->object_data = malloc (sizeof (brk_anim_data));
	
	//Setup the break animation's AH
	animation_handler_set_properties (&(brk_anim->animator), ANIMATION_HANDLER_STILL_FRAME, 1);
	
	//Return the brk_anim
	return brk_anim;
	
}

