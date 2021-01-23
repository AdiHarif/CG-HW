#version 150

in vec3 TexCoords;

uniform samplerCube skybox;

out vec4 fColor;

void main() 
{
	fColor = texture(skybox, TexCoords);
}

