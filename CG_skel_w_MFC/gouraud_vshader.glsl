#version 150

in vec4 v_position;
in vec4 v_normal;

out vec4 color;

uniform mat4 v_transform;
uniform mat4 n_transform;


void main()
{
    gl_Position = v_transform * v_position;

    vec4 tr_v_normal = normalize(n_transform * v_normal);

    vec4 final_ambient_color = vec4(0,0,0,0);
	vec4 final_diffuse_color = vec4(0,0,0,0);
	vec4 final_specular_color = vec4(0,0,0,0);

	//delete these later, add them as input
	//-------------
	vec4 scene_ambient_color = vec4(0, 0, 0, 1);
	vec4 face_emit_color = vec4(0, 0, 0, 1);
	vec4 face_ambient_color = vec4(0, 0, 0, 1);
	vec4 face_diffuse_color = vec4(0, 0.5, 0, 1);
	vec4 face_specular_color = vec4(0.5, 0.5, 0, 1);
	
	vec4 light_dir_1 = vec4(0, 1, 0, 1);
	vec4 light_color_1 = vec4(0, 0.6, 0, 1);
	vec4 light_pos_2 = vec4(1, 0, 0, 1);
	vec4 light_color_2 = vec4(0, 0.6, 0, 1);
	vec4 dir_to_camera = vec4(0, 0, 1, 0);
	float shininess = 0.3;
	//-------------
	
	//calculate ambient:
	final_ambient_color += scene_ambient_color * face_ambient_color;

	
	//calculate diffuse:
	float factor = clamp(dot(normalize(tr_v_normal), normalize(light_dir_1)), 0, 1);
	final_diffuse_color += (face_diffuse_color * light_color_1)*factor;
	
	
	//calculate specular:
	vec4 l = -normalize(gl_Position - light_pos_2);
	l.w = 0;
	vec4 r = 2 * (l * tr_v_normal) * tr_v_normal - l;
	r.w = 0;
	r = normalize(r);
	dir_to_camera.w = 0;
	dir_to_camera = normalize(dir_to_camera);
	float tmp = dot(r, dir_to_camera);
	factor = pow(max(0, tmp),1/shininess);
	final_specular_color += (face_specular_color * light_color_2) * factor;

	
	color = face_emit_color + final_ambient_color + final_diffuse_color + final_specular_color;
	color.w = 1;
}
