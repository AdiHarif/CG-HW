#version 150


in vec4 vPosition;

uniform mat4 tm;
uniform mat4 tc;
uniform mat4 tp;

void main()
{
    gl_Position = tm * vPosition;
}
