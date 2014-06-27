#version 400

layout(location=0) in vec4 in_pos;
layout(location=1) in vec4 in_col;

out vec4 ex_col;

void main(void)
{
	gl_Position = in_pos;
	ex_col = in_col;
} 
