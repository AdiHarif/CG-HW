#version 150


in vec4 v_position;

uniform mat4 v_transform;
uniform mat4 n_transform;


void main()
{
    gl_Position = v_transform * v_position;
}
