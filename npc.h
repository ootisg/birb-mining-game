#ifndef NPC_H
#define NPC_H

#define MAX_NPCS 256

#include "game_object.h"

struct npc_slot {
	game_object* npc;
	int persistent;
};

struct npc_data {
	char* npc_type;
	void* data;
};

typedef struct npc_slot npc_slot;
typedef struct npc_data npc_data;

void init_npcs ();
void init_npc_slot (void* slot);
game_object* alloc_npc (int persistent);
void free_npc (game_object* npc);
npc_data* make_npc_data (void* ptr, char* type, void* data);

#endif