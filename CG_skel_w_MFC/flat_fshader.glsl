#version 150

//in vec4 tr_f_normal;
in vec4 v_position;


out vec4 fColor;

void main() 
{
	vec4 final_ambient_color = vec4(0,0,0,0);
	vec4 final_diffuse_color = vec4(0,0,0,0);

	
	//delete these later, add them as input
	//-------------
	vec4 tr_f_normal = vec4(1,1,1,1);

	vec4 scene_ambient_color = vec4(0.2, 0, 0, 1);
	vec4 face_ambient_color = vec4(1, 0, 0, 1);
	vec4 face_diffuse_color = vec4(0.1, 0.1, 0.1, 1);
	
	vec4 light_dir = vec4(0.5, 0, 0, 1);
	vec4 light_color = vec4(0, 0.5, 0, 1);	
	//-------------
	
	
	//calculate ambient:
	final_ambient_color += scene_ambient_color * face_ambient_color;

	
	//calculate diffuse:
	float factor = dot(tr_f_normal, light_dir);
	if(factor<0){
		factor=0;
	}
	final_diffuse_color += (face_diffuse_color * light_color)*factor;


	fColor = final_ambient_color + final_diffuse_color;
	fColor.w = 1;
} 