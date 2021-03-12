#version 330

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texcoord;
layout (location = 2) in uint texid;

out vec2 texcoord_final;
out uint texid_final;

void main()
{
    gl_Position = vec4(position, 1.0);
	gl_Position.x = (gl_Position.x * 2) - 1; //Transform x to screen coords
	gl_Position.y = ((gl_Position.y * 2) - 1) * -1; //Transform y to screen coords
	texcoord_final = texcoord;
	texid_final = texid;
}