#version 150


in vec4 v_position;
in vec4 v_normal;

uniform mat4 v_transform;
uniform mat4 n_transform;

out vec4 vPosition;
out vec4 vNormal;

void main()
{
    gl_Position = v_transform * v_position;
    vPosition = gl_Position;

	vec4 v_normal = vec4(v_normal.xyz, 0);
    vNormal = normalize(n_transform * v_normal); 
}