#vars for game files
game_deps = game birb viewport tile_map npc game_ui
game_files = game.o birb.o viewport.o tile_map.o npc.o game_ui.o
#build the engine
main: main.c $(game_deps) inputs sprite layout render geometry char_buffer linked_list hash_table json object_handler game_object stack gui glew lodepng
	gcc -g -Llib main.c $(game_files) inputs.o sprite.o layout.o render.o geometry.o char_buffer.o linked_list.o hash_table.o json.o object_handler.o game_object.o stack.o gui.o glew.o lodepng.o -DGLEW_STATIC -lopengl32 -lglu32 -lfreeglut -o main.exe
inputs: inputs.c inputs.h
	gcc -g inputs.c -c -LGL -Llib -lopengl32 -lglu32 -lfreeglut -lglew32
sprite: sprite.c sprite.h
	gcc -g sprite.c -c
layout: layout.c layout.h
	gcc -g layout.c -c
render: render.c render.h texture_mapping.h
	gcc -g render.c -c
geometry: geometry.c geometry.h
	gcc -g geometry.c -c
stack: stack.c stack.h
	gcc -g stack.c -c
char_buffer: char_buffer.c char_buffer.h
	gcc -g char_buffer.c -c
linked_list: linked_list.c linked_list.h
	gcc -g linked_list.c -c
hash_table: hash_table.c hash_table.h
	gcc -g hash_table.c -c
json: json.c json.h
	gcc -g json.c -c
object_handler: object_handler.c object_handler.h
	gcc -g object_handler.c -c
game_object: game_object.c game_object.h
	gcc -g game_object.c -c
gui: gui.c gui.h
	gcc -g gui.c -c
glew: glew.c
	gcc -c -g glew.c -DGLEW_STATIC -mwindows
lodepng: lodepng.c lodepng.h
	gcc -c -g lodepng.c
#builds for game files
game: game.c game.h
	gcc -g game.c -c
birb: birb.c birb.h
	gcc -g birb.c -c
viewport: viewport.c viewport.h
	gcc -g viewport.c -c
tile_map: tile_map.c tile_map.h
	gcc -g tile_map.c stb_perlin.h -c
npc: npc.c npc.h
	gcc -g npc.c -c
game_ui: game_ui.c game_ui.h
	gcc -g game_ui.c -c
