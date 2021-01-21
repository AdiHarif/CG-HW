#version 150

in vec4 vPosition;
in vec4 vNormal;

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

out vec4 fColor;

void main() 
{

	vec4 actual_light_dir;

	//caculate actual light dir:
	if(is_point_source){
		actual_light_dir = light_pos - vPosition;
	}
	else{
		actual_light_dir = light_dir;
	}

	actual_light_dir.w = 0;

	//calculate diffuse:
	float diffuse_factor = clamp(dot(normalize(vNormal), normalize(actual_light_dir)), 0, 1);
	vec4 diffuse_color = (model_diffuse_color * light_color)*diffuse_factor;


	//calculate dir to camera:
	vec4 dir_to_camera = camera_pos - vPosition;

	vec4 specular_color = vec4(0, 0, 0, 1);

	if(is_point_source){
		//calculate specular:
		vec4 l = normalize(actual_light_dir);
		l.w = 0;
		vec4 n = vNormal;
		vec4 r = 2 * dot(l, n) * n - l;
		r.w = 0;
		r = normalize(r);
		dir_to_camera.w = 0;
		dir_to_camera = normalize(dir_to_camera);
		float tmp = dot(r, dir_to_camera);
		float specular_factor = pow(max(0, tmp),1/shininess);
		specular_color = (model_specular_color * light_color) * specular_factor;
	}

	fColor = diffuse_color + specular_color;
	fColor.w = 1;
} 