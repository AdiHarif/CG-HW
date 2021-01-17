#version 150


in vec4 v_position;
in vec4 f_normal;

out vec4 color;

uniform mat4 v_transform;
uniform mat4 n_transform;


void main()
{
    gl_Position = v_transform * v_position;
    vec4 tr_f_normal = n_transform * f_normal;

    vec4 final_ambient_color = vec4(0,0,0,0);
	vec4 final_diffuse_color = vec4(0,0,0,0);

	//delete these later, add them as input
	//-------------
	vec4 scene_ambient_color = vec4(0.2, 0, 0, 1);
	vec4 face_emit_color = vec4(0, 0, 0.1, 1);
	vec4 face_ambient_color = vec4(1, 0, 0.2, 1);
	vec4 face_diffuse_color = vec4(0.2, 0, 0.5, 1);
	
	vec4 light_dir = vec4(0, 0, 0, 1);
	vec4 light_color = vec4(0.1, 0.1, 0.2, 1);	
	//-------------
	
	//calculate ambient:
	final_ambient_color += scene_ambient_color * face_ambient_color;

	
	//calculate diffuse:
	float factor = dot(normalize(tr_f_normal), normalize(light_dir));
	if(factor<0){
		factor=0;
	}
	final_diffuse_color += (face_diffuse_color * light_color)*factor;
	
	color = face_emit_color + final_ambient_color + final_diffuse_color;
	color.w = 1;
}
