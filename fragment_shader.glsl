#version 330

uniform sampler2D texture;

in vec2 texcoord_final;
flat in uint texid_final;

void main()
{
   gl_FragColor = texture2D (texture, texcoord_final);
}