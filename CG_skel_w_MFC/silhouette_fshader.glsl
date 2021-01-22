#version 150

out vec4 fColor;

void main() 
{
	if (gl_FrontFacing){
		discard;
	}
	fColor = vec4(0,0,0,1);
	
} 