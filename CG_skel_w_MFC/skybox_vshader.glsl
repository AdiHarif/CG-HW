#version 150

in vec4 v_position;

uniform mat4 tp;
uniform mat4 tc;
uniform mat4 tw;
uniform mat4 tm;

out vec3 TexCoords;

void main()
{
    TexCoords = v_position.xyz;
    gl_Position = tp * mat4(mat3(tc)) * tw * vec4(v_position.xyz, 1.0);
}

