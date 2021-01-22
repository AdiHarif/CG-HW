#version 150

in vec2 texCoord;

uniform sampler2D modelTexture;

out vec4 fColor;

void main() 
{
	fColor = texture(modelTexture, texCoord);
} 