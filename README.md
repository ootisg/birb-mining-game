This is a small game I made to test out my [2D game engine in c](https://github.com/ootisg/pure-c-game-engine).  
This was initially planned to be a sidescrolling excavation game. I later opted to go with a top-down game instead.  
  
Instructions for building: 
  
This game has the following dependancies:  
	- Must be compiled with MinGW
	- Win32 (this is a WINDOWS ONLY library)  
	- OpenGL version 3.0 or higher  
	- GLU  
	- freeglut  
	- GLEW (put the source glew.c file in the project directory, header can go wherever as long as it's accessable)  
	- lodepng (put the .c and .h files in the project directory)  
	- stb_perlin.h (put in the project directory)
  
Steps to build:  
	Note: This project includes a lib and an include directory in the source folder. Libraries/headers can either be placed here, or in your system's lib/include folders. It doesn't particularly matter which.
	1) Download and install OpenGL if you haven't already  
	2) Make sure you have MinGW installed along with GNUMake.  
	3) Download and install GLU and freeglut (these libraries should be in your lib directory)  
	4) Download glew.c and glew.h, and put glew.c in the project's source directory. glew.h can go in your include directory.  
	5) Download lodepng.c and lodepng.h, and put them both in the project's source directory.  
	6) Build with gnu make using the provided Makefile.  
  
You control a small bird character with the WASD keys. You can also open/close the inventory with the e key, and the mouse is used to navigate the UI menus.  
The world is fixed-size and procedurally generated with perlin noise. There are currently 2 biomes and multiple ores in each biome, also generated using perlin noise.  
The gameplay cycle involves mining ores, selling them (yet to be implemented) at the shop, then buying upgrades to mine more ores. Resource management and building elements are planned to be added eventually, but may ultimately be scrapped.  
Also, there's a strange bug that causes some sprites/tiles to be slightly misaligned on certain hardware. These show up as seams and are not very pretty. I've tested this a fair bit and it doesn't seem to be a floating point precision error. If anyone knows how to fix this, please let me know.  