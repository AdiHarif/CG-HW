#version 150


in vec4 v_position;
in vec4 f_normal;

in vec4 face_diffuse_color;
in vec4 face_specular_color;
in float shininess;

in vec4 light_color;
in vec4 light_pos;
in vec4 light_dir;
in vec4 camera_pos;

in bool is_point_source;

out vec4 color;

uniform mat4 v_transform;
uniform mat4 n_transform;

void main()
{	
    gl_Position = v_transform * v_position;
	vec4 f_normal = vec4(f_normal.xyz, 0);
    vec4 tr_f_normal = normalize(n_transform * f_normal);


	//caculate light dir:
	if(is_point_source){
		light_dir = light_pos - gl_Position;
	}


	//calculate diffuse:
	float factor = clamp(dot(normalize(tr_f_normal), normalize(face_to_light_dir)), 0, 1);
	vec4 diffuse_color = (face_diffuse_color * light_color)*factor;


	//calculate dir to camera:
	vec4 dir_to_camera = camera_pos - gl_Position;

	//calculate specular:
	vec4 l = -normalize(light_dir);
	l.w = 0;
	vec4 n = f_normal;
	vec4 r = 2 * (l * n) * n - l;
	r.w = 0;
	r = normalize(r);
	dir_to_camera.w = 0;
	dir_to_camera = normalize(dir_to_camera);
	float tmp = (r * dir_to_camera);
	float factor = pow(max(0, tmp),1/shininess);
	specular_color = (face_specular_color * light_color) * factor;
	

	color = diffuse_color + specular_color;
	color.w = 1;
}
