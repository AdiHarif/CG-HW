#version 150


in vec4 v_position;

uniform mat4 v_transform;

out vec2 texCoord;

void main()
{
    gl_Position = v_transform * v_position;
    gl_Position.w = 1;
    texCoord = v_position.xy;
}
