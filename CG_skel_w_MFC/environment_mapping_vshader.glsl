#version 150

in vec4 v_position;
in vec4 v_normal;

uniform mat4 tp;
uniform mat4 tc;
uniform mat4 tw;
uniform mat4 tm;

out vec4 normal;
out vec4 position;

void main() 
{
	normal = transpose(inverse(tw*tm)) * v_normal;
	position = tw * tm * vec4(v_position.xyz, 1.0);
	position.w = 1;
	gl_Position = tp * tc * position;
}
