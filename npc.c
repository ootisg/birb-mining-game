#include "npc.h"

npc_slot* npcs;
int buffer_pos;

#include "object_handler.h"
#include "stack.h"
#include "tile_map.h"
#include "game.h"

void init_npcs () {
	
	//Initialize the buffer
	npcs = malloc (sizeof (npc_slot) * MAX_NPCS);
	buffer_pos = 0;
	
	//Make the NPCs
	int i;
	for (i = 0; i < MAX_NPCS; i++) {
		game_object* curr = make_game_object (malloc (sizeof (game_object)), "NPC");
		curr->draw_call = viewport_draw;
		declare_game_object (get_global_object_handler (), curr);
		curr->x = -1;
		curr->y = -1;
		curr->width = MAP_TILE_SIZE;
		curr->height = MAP_TILE_SIZE;
		npcs[i].npc = curr;
		npcs[i].persistent = 0;
	}
	
}

void init_npc_slot (void* slot) {
	((npc_slot*)slot)->npc = NULL;
	((npc_slot*)slot)->persistent = 0;
}

game_object* alloc_npc (int persistent) {
	
	//Find a usable slot
	int buffer_start = ++buffer_pos;
	while (npcs[buffer_pos % MAX_NPCS].persistent) {
		buffer_pos++;
		if (buffer_pos - buffer_start > MAX_NPCS) {
			buffer_pos = buffer_pos % MAX_NPCS;
			return NULL;
		}
	}
	
	//Get the npc from the slot
	buffer_pos = buffer_pos % MAX_NPCS;
	npcs[buffer_pos].persistent = persistent;
	return npcs[buffer_pos].npc;
	
}

void free_npc (game_object* npc) {
	//TODO different stuff
	npc->x = -1;
	npc->y = -1;
}