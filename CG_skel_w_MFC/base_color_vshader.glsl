#version 150


in vec4 scene_ambient_color;
in vec4 face_ambient_color;

out vec4 color;


void main()
{
    color = scene_ambient_color * face_ambient_color;
}