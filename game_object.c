#include "game_object.h"

int default_is_colliding (game_object* this, game_object* other) {
	if (rectangle_overlaps (&(this->hitbox), &(other->hitbox))) {
		return 1;
	}
	return 0;
}

void generate_hitbox (game_object* obj) {
	make_rectangle (&(obj->hitbox), obj->x, obj->y, obj->width, obj->height);
}

game_object* make_game_object (void* ptr, char* obj_type) {
	game_object* obj = (game_object*)ptr;
	obj->object_type = obj_type;
	obj->vertex_data = alloc_render_object ();
	obj->init_call = default_init;
	obj->draw_call = default_draw;
	obj->game_logic_call = default_game_logic;
	obj->is_colliding = default_is_colliding;
	make_animation_handler (&(obj->animator));
	return obj;
}

void default_init (game_object* obj) {
	//THIS IS FOR TESTING
	obj->sprite = make_sprite ("testure.png");
	obj->x = 0;
	obj->y = 0;
	obj->width = .5;
	obj->height = .5;
}

void default_game_logic (game_object* obj) {
	//Intentionally left empty
}

animation_handler* make_animation_handler (void* ptr) {
	animation_handler* h_ptr = (animation_handler*)ptr;
	h_ptr->flags = ANIMATION_HANDLER_TYPE_MS | ANIMATION_HANDLER_LOOP_REPEAT;
	h_ptr->start_time = get_frame_time_ms ();
	h_ptr->frame_count = 0;
	h_ptr->frame_time = 17;
	return h_ptr;
}

int animation_handler_get_frame (animation_handler* ptr) {
	int anim_frame;
	if ((ptr->flags) & ANIMATION_HANDLER_TYPE) {
		//Animation by ms
		anim_frame = (int)((get_frame_time_ms () - ptr->start_time) / ptr->frame_time);
	} else {
		//Animation by frames
		anim_frame = (int)((get_frame_count () - ptr->start_time) / ptr->frame_time);
	}
	if ((ptr->flags) & ANIMATION_HANDLER_LOOP) {
		//Animation loops
		return anim_frame % ptr->frame_count;
	} else {
		//Animation does not loop
		if (anim_frame < ptr->frame_count - 1) {
			//Just return the frame
			return anim_frame;
		} else {
			//Animation is finished
			ptr->flags |= ANIMATION_HANDLER_FINISHED;
			return ptr->frame_count - 1;
		}
	}
}

void animation_handler_set_properties (animation_handler* ptr, int properties, int val) {
	if (val) {
		ptr->flags |= properties;
	} else {
		ptr->flags &= ~properties;
	}
}

int animation_handler_get_properties (animation_handler* ptr, int property) {
	return (ptr->flags) & property;
}

void default_draw (game_object* obj) {
	if (obj->sprite) {
		
		//Get the object's position and sprite data
		obj->animator.frame_count = obj->sprite->frame_count;
		int draw_frame = animation_handler_get_frame (&(obj->animator));
		rectangle* r = &(obj->sprite->frames[draw_frame]);
		unsigned int tex_id = obj->sprite->mapping->tex_id;
		
		//Update the object's vertex data
		//Top-left
		obj->vertex_data[0] = obj->x;
		obj->vertex_data[1] = obj->y;
		obj->vertex_data[3] = r->x;
		obj->vertex_data[4] = r->y;
		obj->vertex_data[5] = tex_id;
		//Bottom-left
		obj->vertex_data[6] = obj->x;
		obj->vertex_data[7] = obj->y + obj->height;
		obj->vertex_data[9] = r->x;
		obj->vertex_data[10] = r->y + r->height;;
		obj->vertex_data[11] = tex_id;
		//Bottom-right
		obj->vertex_data[12] = obj->x + obj->width;
		obj->vertex_data[13] = obj->y + obj->height;
		obj->vertex_data[15] = r->x + r->width;
		obj->vertex_data[16] = r->y + r->height;
		obj->vertex_data[17] = tex_id;
		//Top-right
		obj->vertex_data[18] = obj->x + obj->width;
		obj->vertex_data[19] = obj->y;
		obj->vertex_data[21] = r->x + r->width;
		obj->vertex_data[22] = r->y;
		obj->vertex_data[23] = tex_id;
	}
}