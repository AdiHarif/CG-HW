#version 150

out vec4 fColor;

uniform vec4 model_emit_color;
uniform vec4 model_ambient_color;
uniform vec4 scene_ambient_color;

void main() 
{
	fColor = model_ambient_color * scene_ambient_color + model_emit_color;
}