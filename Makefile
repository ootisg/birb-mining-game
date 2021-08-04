#vars for game files
game_deps = game birb viewport tile_map npc game_ui
game_files = game.o birb.o viewport.o tile_map.o npc.o game_ui.o
#build the engine
main: main.c $(game_deps) inputs sprite layout render geometry char_buffer linked_list hash_table json object_handler game_object stack gui glew lodepng
	gcc -Llib main.c $(game_files) inputs.o sprite.o layout.o render.o geometry.o char_buffer.o linked_list.o hash_table.o json.o object_handler.o game_object.o stack.o gui.o glew.o lodepng.o -DGLEW_STATIC -lopengl32 -lglu32 -lfreeglut -o main.exe
inputs: inputs.c inputs.h
	gcc inputs.c -c -LGL -Llib -lopengl32 -lglu32 -lfreeglut -lglew32
sprite: sprite.c sprite.h
	gcc sprite.c -c
layout: layout.c layout.h
	gcc layout.c -c
render: render.c render.h texture_mapping.h
	gcc render.c -c
geometry: geometry.c geometry.h
	gcc geometry.c -c
stack: stack.c stack.h
	gcc stack.c -c
char_buffer: char_buffer.c char_buffer.h
	gcc char_buffer.c -c
linked_list: linked_list.c linked_list.h
	gcc linked_list.c -c
hash_table: hash_table.c hash_table.h
	gcc hash_table.c -c
json: json.c json.h
	gcc json.c -c
object_handler: object_handler.c object_handler.h
	gcc object_handler.c -c
game_object: game_object.c game_object.h
	gcc game_object.c -c
gui: gui.c gui.h
	gcc gui.c -c
glew: glew.c
	gcc -c glew.c -DGLEW_STATIC -mwindows
lodepng: lodepng.c lodepng.h
	gcc -c lodepng.c
#builds for game files
game: game.c game.h
	gcc game.c -c
birb: birb.c birb.h
	gcc birb.c -c
viewport: viewport.c viewport.h
	gcc viewport.c -c
tile_map: tile_map.c tile_map.h
	gcc tile_map.c stb_perlin.h -c
npc: npc.c npc.h
	gcc npc.c -c
game_ui: game_ui.c game_ui.h
	gcc game_ui.c -c
