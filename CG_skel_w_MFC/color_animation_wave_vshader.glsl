#version 150


in vec4 v_position;

uniform float wave_threshold;
uniform vec3 color0;
uniform vec3 color1;
uniform mat4 v_transform;

out vec3 color;

void main()
{
    vec4 position = v_transform * v_position;
    position.w = 1;
    gl_Position = position;
    
    if(position.x < wave_threshold){
        color = color0;
    }
    else{
        color = color1;
    }
}
