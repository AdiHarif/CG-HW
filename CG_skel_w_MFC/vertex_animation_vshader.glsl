#version 150


in vec4 v_position;

uniform float vertex_animation_t;
uniform mat4 v_transform;

out vec3 color;

void main()
{
    gl_Position = v_transform * v_position;
    vec4 dir_to_zero = -v_position;
    vec4 offset_towards_zero = vertex_animation_t * dir_to_zero;
    
    gl_Position = gl_Position + offset_towards_zero;
    gl_Position.w = 1;

    color = vec3(1, 1, 1); // TODO: change to dynamic color?
}
