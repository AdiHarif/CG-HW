#version 150

in vec4 position;

uniform mat4 twm;
uniform mat4 tpc;

void main()
{
	gl_Position = tpc*twm*position;
}
