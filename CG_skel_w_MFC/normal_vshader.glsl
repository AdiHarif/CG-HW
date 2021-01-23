#version 150


in vec4 position;
in vec4 normal;

uniform mat4 twm;
uniform mat4 tpc;
uniform mat4 twm_n;

void main()
{
	if (normal.w == 0){
		gl_Position = tpc * twm * position;
		gl_Position = gl_Position/gl_Position.w;
	}
	else{
		vec4 wr_position = (twm*position);
		//wr_position = wr_position/wr_position.w;

		vec4 wr_normal = (twm_n*normal);
		//wr_normal = wr_normal/wr_normal.w;
		//wr_normal.w = 0;
		wr_normal = -normalize(wr_normal)*0.1;
		//wr_normal.w = 1;
		
		gl_Position = tpc * (wr_position + wr_normal);
		//gl_Position = gl_Position/gl_Position.w;
	}
}
