#include <stdlib.h>

#include "sprite.h"

unsigned char** text_chars = NULL;

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
	int y1 = (int)(bounds->y * TEXTURE_SIZE); //Flip the y position to accomodate texture stuff(?)
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

void draw_to_sprite (sprite* spr, unsigned char* source, int frame, float x, float y, int source_width, int source_height) {
	
	//Setup the important data
	rectangle* bounds = &(spr->frames[frame]);
	unsigned int tex_id = spr->mapping->tex_id;
	char* dest = get_sprite_texture (tex_id);
	int* source_int = (int*)source;
	int* dest_int = (int*)dest;
	
	//Calculate the bounds
	int x1 = (int)((bounds->x + (x * bounds->width)) * TEXTURE_SIZE);
	int y1 = (int)((bounds->y + (y * bounds->height)) * TEXTURE_SIZE);
	
	//Copy the image data over
	int wx, wy;
	for (wx = 0; wx < source_width; wx++) {
		for (wy = 0; wy < source_height; wy++) {
			if (source_int [(wy) * source_width + wx] && 0xFF000000) {
				dest_int[(wy + y1) * TEXTURE_SIZE + wx + x1] = source_int [(wy) * source_width + wx];
			}
		}
	}
	
	//Refresh the sprite texture
	refresh_sprite_textures ();
	
}

void sprite_fill_rect (sprite* spr, int color, int frame, float x, float y, float width, float height) {
	
	//Get the size of the rect to draw in px
	rectangle* bounds = &(spr->frames[frame]);
	int width_px = (int)(width * bounds->width * TEXTURE_SIZE);
	int height_px = (int)(height * bounds->height * TEXTURE_SIZE);
	
	//Fill a buffer with color data
	int* rect_pxs = malloc (sizeof (int) * width_px * height_px);
	int i;
	for (i = 0; i < width_px * height_px; i++) {
		rect_pxs [i] = color;
	}
	
	//Draw to the sprite
	draw_to_sprite (spr, (unsigned char*)rect_pxs, frame, x, y, width_px, height_px);
	
	//Free the buffer
	free (rect_pxs);
	
}

void img_copy (unsigned char* src, int src_width, int src_height, unsigned char* dest, int dest_width, int dest_height, int src_reigon_x, int src_reigon_y, int dest_reigon_x, int dest_reigon_y, int copy_width, int copy_height) {
	int wx, wy;
	for (wx = 0; wx < copy_width; wx++) {
		for (wy = 0; wy < copy_height; wy++) {
			int src_index = (src_reigon_y + wy) * src_width + (src_reigon_x + wx);
			int dest_index = (dest_reigon_y + wy) * dest_width + (dest_reigon_x + wx);
			((int*)dest)[dest_index] = ((int*)src)[src_index];
		}
	}
}

unsigned char* text_get_char (char c) {
	
	//Initialize the text data if needed
	if (!text_chars) {
		
		//Decode the chars file
		int width, height;
		unsigned char* text_data;
		lodepng_decode32_file (&text_data, &width, &height, "resources/sprites/font.png");
		
		//Check the dimensions of the text
		if (width != TEXT_DATA_WIDTH || height != TEXT_DATA_HEIGHT) {
			printf ("Error: text file does not have the correct dimensions (should be %dx%d)\n", TEXT_DATA_WIDTH, TEXT_DATA_HEIGHT);
			exit (1);
		}
		
		//Make the chars list
		text_chars = malloc (sizeof (char*) * 0x5F);
		
		//Copy the text data over
		int wx, wy, i;
		i = 0;
		for (wy = 0; wy < 0x6; wy++) {
			for (wx = 0; wx < 0x10; wx++) {
				int char_x = wx * TEXT_CHAR_WIDTH;
				int char_y = wy * TEXT_CHAR_HEIGHT;
				text_chars[i] = malloc (sizeof (int) * TEXT_CHAR_WIDTH * TEXT_CHAR_HEIGHT);
				img_copy (text_data, TEXT_DATA_WIDTH, TEXT_DATA_HEIGHT, text_chars[i], TEXT_CHAR_WIDTH, TEXT_CHAR_HEIGHT, wx * TEXT_CHAR_WIDTH, wy * TEXT_CHAR_HEIGHT, 0, 0, TEXT_CHAR_WIDTH, TEXT_CHAR_HEIGHT);
				i++;
			}
		}
		
	}
	
	//Return the char data
	return text_chars[c - 0x20];
	
}

void sprite_draw_char (sprite* spr, int frame, float x, float y, char c) {
	
	unsigned char* char_buffer = text_get_char (c);
	draw_to_sprite (spr, char_buffer, frame, x, y, TEXT_CHAR_WIDTH, TEXT_CHAR_HEIGHT);
	
}

void sprite_draw_string (sprite* spr, int frame, float x, float y, char* str) {
	
	rectangle* bounds = &(spr->frames[frame]);
	int pixel_x = (int)((bounds->x + x * bounds->width) * TEXTURE_SIZE);
	int i = 0;
	while (str[i]) {
		float f_x = (float)pixel_x / TEXTURE_SIZE;
		float actual_x = (f_x - bounds->x) / bounds->width;
		sprite_draw_char (spr, frame, actual_x, y, str[i]);
		pixel_x += TEXT_CHAR_WIDTH;
		i++;
	}
	
}