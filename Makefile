main: main.c inputs sprite layout render geometry char_buffer linked_list hash_table json object_handler game_object stack glew lodepng
	gcc main.c inputs.o sprite.o layout.o render.o geometry.o char_buffer.o linked_list.o hash_table.o json.o object_handler.o game_object.o stack.o glew.o lodepng.o -DGLEW_STATIC -lopengl32 -lglu32 -lfreeglut -lglew32 -o main.exe
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
glew: glew.c
	gcc -c glew.c -DGLEW_STATIC -mwindows
lodepng: lodepng.c lodepng.h
	gcc -c lodepng.c