#include <stdlib.h>

#include "sprite.h"

sprite* make_sprite_from_json (char* json_path, char* sprite_path) {
	
	//Read JSON file (if non-null)
	json_object* params = NULL;
	layout_element* layout = NULL;
	json_object* json_bounds_obj = NULL;
	if (json_path) {
		params = read_json_file (json_path);
		if (!params) {
			printf ("ERROR: %s is not a valid JSON file\n", json_path);
			exit (1);
		}
		//Read the sprite_path value if present
		json_object* json_sprite_path = json_get_object (params, "sprite_path");
		json_bounds_obj = json_get_object (params, "layout");
		if (json_sprite_path) {
			//Read the sprite_path value if present
			sprite_path = json_sprite_path->token_data;
		}
		if (json_bounds_obj) {
			layout = make_layout (malloc (sizeof (layout_element)), params);
		}
	}
	
	//Read from the input file
	sprite* out_sprite = malloc (sizeof (sprite));
	int width, height;
	unsigned char* img;
	lodepng_decode32_file (&img, &width, &height, sprite_path);
	
	//Transfer the sprite data to a sprite texture
	texture_mapping* map = get_texture_mapping (width, height);
	out_sprite->mapping = map;
	transfer_to_texture (map, img, width, height);
	
	//Set the sprite properties
	out_sprite->image_data = img;
	out_sprite->texture_id = map->tex_id;
	out_sprite->width_px = width;
	out_sprite->height_px = height;
	out_sprite->width = map->bounds.width;
	out_sprite->height = map->bounds.height;
	if (layout) {
		linked_list* frames_list = get_layout_reigons (layout, &(map->bounds));
		printf ("%d\n", frames_list->size);
		out_sprite->frame_count = frames_list->size; //TODO
		rectangle* frames = malloc (sizeof (rectangle) * frames_list->size);
		linked_list_node* curr = frames_list->head;
		int i = 0;
		while (i < frames_list->size) {
			print_rectangle (curr->node_data);
			snap_to_textures (frames + i, curr->node_data);
			i++;
			curr = curr->next;
		}
		out_sprite->frames = frames;
		//Free all the resources that were used
		free_layout (layout);
		free_linked_list_elements (frames_list, NULL);
		free (frames_list);
	} else {
		out_sprite->frames = &(map->bounds);
		out_sprite->frame_count = 1;
	}
	return out_sprite;
}

sprite* make_sprite (char* filepath) {
	
	make_sprite_from_json (NULL, filepath);
	
}

void transfer_to_texture (texture_mapping* map, unsigned char* source, int source_width, int source_height) {

	//Setup the important data
	rectangle* bounds = &(map->bounds);
	unsigned int tex_id = map->tex_id;
	char* dest = get_sprite_texture (tex_id);
	int* source_int = (int*)source;
	int* dest_int = (int*)dest;
	
	//Calculate the x bounds
	int x1 = (int)(bounds->x * TEXTURE_SIZE);
	int y1 = (int)(bounds->y * TEXTURE_SIZE); //Flip the y position to accomodate texture stuff
	int x2 = x1 + source_width; //TODO unused
	int y2 = y2 - source_height;
	
	//Copy over
	int wx, wy;
	for (wy = 0; wy < source_height; wy++) {
		for (wx = 0; wx < source_width; wx++) {
			dest_int[(wy + y1) * TEXTURE_SIZE + wx + x1] = source_int [(wy) * source_width + wx];
		}
	}
	
	//Refresh the sprite texture
	refresh_sprite_textures ();
}