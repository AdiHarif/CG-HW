#version 150


in vec4 v_position;

uniform vec3 hsv_color;
uniform mat4 v_transform;

out vec3 color;

void main()
{
    gl_Position = v_transform * v_position;
    gl_Position.w = 1;

    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(hsv_color.xxx + K.xyz) * 6.0 - K.www);
    color = (hsv_color.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), hsv_color.y));
}
