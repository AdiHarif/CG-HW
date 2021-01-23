#version 150

out vec4 fColor;

in vec4 normal;
in vec4 position;

uniform vec4 camera_pos;
uniform samplerCube skybox;

void main()
{
	vec4 I = normalize(position - camera_pos);
	vec4 R = reflect(I, normalize(normal));
	fColor = vec4(texture(skybox, R.xyz).rgb, 1.0);
}
