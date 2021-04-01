#include "npc.h"

stack* all_npcs;
stack* free_npcs;

#include "object_handler.h"
#include "stack.h"
#include "game.h"

void init_npcs () {
	
	//Make the NPC stacks
	all_npcs = init_stack (sizeof (game_object*));
	free_npcs = init_stack (sizeof (game_object*));
	
	//Make the NPCs
	int i;
	for (i = 0; i < MAX_NPCS; i++) {
		game_object* curr = make_game_object (malloc (sizeof (game_object)), "NPC");
		//curr->draw_call = viewport_draw;
		declare_game_object (get_global_object_handler (), curr);
		push (all_npcs, &curr);
		push (free_npcs, &curr);
	}
	
}

game_object* alloc_npc () {
	return *((game_object**)(pop (free_npcs)));
}

void free_npc (game_object* npc) {
	npc->x = -1;
	npc->y = -1;
	push (free_npcs, &npc);
}