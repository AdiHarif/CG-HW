#version 150


in vec4 v_position;
in vec4 f_normal;


uniform vec4 model_diffuse_color;
uniform vec4 model_specular_color;
uniform float shininess;

uniform vec4 light_color;
uniform vec4 light_pos;
uniform vec4 light_dir;
uniform vec4 camera_pos;
uniform bool is_point_source;

uniform mat4 v_transform;
uniform mat4 n_transform;


out vec4 color;

void main()
{	
    gl_Position = v_transform * v_position;
	vec4 f_normal = vec4(f_normal.xyz, 0);
    vec4 tr_f_normal = normalize(n_transform * f_normal);

	vec4 actual_light_dir;

	//caculate actual light dir:
	if(is_point_source){
		actual_light_dir = light_pos - gl_Position;
	}
	else{
		actual_light_dir = light_dir;
	}

	actual_light_dir.w = 0;

	//calculate diffuse:
	float diffuse_factor = clamp(dot(normalize(tr_f_normal), normalize(actual_light_dir)), 0, 1);
	vec4 diffuse_color = (model_diffuse_color * light_color)*diffuse_factor;


	//calculate dir to camera:
	vec4 dir_to_camera = camera_pos - gl_Position;

	//calculate specular:
	vec4 l = -normalize(actual_light_dir);
	l.w = 0;
	vec4 n = f_normal;
	vec4 r = 2 * dot(l, n) * n - l;
	r.w = 0;
	r = normalize(r);
	dir_to_camera.w = 0;
	dir_to_camera = normalize(dir_to_camera);
	float tmp = dot(r, dir_to_camera);
	float specular_factor = pow(max(0, tmp),1/shininess);
	vec4 specular_color = (model_specular_color * light_color) * specular_factor;
	

	color = diffuse_color + specular_color;
	color.w = 1;
}
