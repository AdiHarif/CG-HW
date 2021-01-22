#version 150

in vec4 v_position;
in vec4 v_normal;

uniform mat4 twm;
uniform mat4 tpc;
uniform mat4 twm_n;

void main()
{
	vec4 v_normal = vec4(v_normal.xyz, 0);
    vec4 wr_normal = normalize(twm_n * v_normal);
    vec4 wr_vertex = twm * v_position;
    
    gl_Position = tpc*(wr_vertex + (0.06 * wr_normal));
}
