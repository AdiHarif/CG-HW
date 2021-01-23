#version 150


in vec4 v_position;
in vec2 v_map_coordinates;
in vec4 t_axis;
in vec4 b_axis;

uniform mat4 twm;
uniform mat4 tpc;

out vec2 f_map_coordinates;
out vec4 wr_position;
out vec4 f_t_axis;
out vec4 f_b_axis;

void main()
{
    wr_position = twm * v_position;
    gl_Position = tpc * wr_position;
    f_map_coordinates = v_map_coordinates;
    f_t_axis = t_axis;
    f_b_axis = b_axis;
}