#ifndef NPC_H
#define NPC_H

#define MAX_NPCS 256

#include "game_object.h"

struct npc_slot {
	game_object* npc;
	int persistent;
};

typedef struct npc_slot npc_slot;

void init_npcs ();
void init_npc_slot (void* slot);
game_object* alloc_npc (int persistent);
void free_npc (game_object* npc);

#endif