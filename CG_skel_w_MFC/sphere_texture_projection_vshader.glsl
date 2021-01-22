#version 150


in vec4 v_position;

uniform mat4 v_transform;

out vec3 position;

void main()
{
    gl_Position = v_transform * v_position;
    gl_Position.w = 1;
    position = v_position.xyz;
}
