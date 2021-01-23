#version 150

in vec4 wr_position;
in vec2 f_map_coordinates;
in vec4 f_t_axis;
in vec4 f_b_axis;

uniform vec4 model_diffuse_color;
uniform vec4 model_specular_color;
uniform float shininess;
uniform sampler2D normal_map;

uniform vec4 light_color;
uniform vec4 light_pos;
uniform vec4 light_dir;
uniform vec4 camera_pos;
uniform bool is_point_source;

uniform mat4 twm_n;

out vec4 fColor;

void main() 
{
	
    vec4 f_n_axis = normalize(vec4(cross(f_t_axis.xyz, f_b_axis.xyz), 0));
    mat4 tbn = mat4(
        vec4(f_t_axis.xyz, 0),
        vec4(f_b_axis.xyz, 0),
        vec4(f_n_axis.xyz, 0),
        vec4(0,0,0,1)
    );

	vec4 map_color  = texture(normal_map, f_map_coordinates);
	vec4 normal = vec4((map_color.xyz + vec3(1,1,1))*0.5, 1);
	vec4 wr_normal = twm_n * tbn * normal;
	wr_normal = -normalize(wr_normal/wr_normal.w);

	vec4 actual_light_dir;

	//caculate actual light dir:
	if(is_point_source){
		actual_light_dir = light_pos - wr_position;
	}
	else{
		actual_light_dir = light_dir;
	}

	actual_light_dir.w = 0;

	//calculate diffuse:
	float diffuse_factor = clamp(dot(normalize(wr_normal), normalize(actual_light_dir)), 0, 1);
	vec4 diffuse_color = (model_diffuse_color * light_color)*diffuse_factor;


	//calculate dir to camera:
	vec4 dir_to_camera = camera_pos - wr_position;

	vec4 specular_color = vec4(0, 0, 0, 1);

	if(is_point_source){
		//calculate specular:
		vec4 l = normalize(actual_light_dir);
		l.w = 0;
		vec4 n = wr_normal;
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