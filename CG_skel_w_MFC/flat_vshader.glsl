#version 150


in vec4 v_position;
in vec4 f_normal;

out vec4 tr_f_normal;

uniform mat4 v_transform;
uniform mat4 n_transform;


void main()
{
    gl_Position = v_transform * v_position;
    tr_f_normal = n_transform * f_normal;
}
