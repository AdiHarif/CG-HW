#version 150

#define PI 3.141592654

in vec3 position;

uniform sampler2D modelTexture;

out vec4 fColor;

void main() 
{
    float theta = atan(sqrt(position.x*position.x + position.y*position.y) / position.z);
    float phi = atan(position.y/position.x);    
    theta = theta + (PI / 2)/ PI;
    phi = phi + (PI) / (2 * PI);     
    float t;
    float s;
    vec2 tex_coord = vec2(theta, phi); 
    fColor = texture(modelTexture, tex_coord);
} 