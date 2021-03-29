#ifndef NPC_H
#define NPC_H

#define MAX_NPCS 64

#include "game_object.h"

void init_npcs ();
game_object* alloc_npc ();
void free_npc (game_object* npc);

#endif