#version 150


in  vec4 vPosition;



void main()
{
    gl_Position = vPosition;
    gl_Position.w = 0.5;
}