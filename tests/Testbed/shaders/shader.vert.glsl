#version 450
#pragma shader_stage(vertex)

layout (location = 0) in vec3 in_position;
layout (location = 1) in vec3 in_color;

layout (location = 0) out vec3 out_color;
layout (location = 1) out vec3 out_position;

void main(){
	out_color = in_color;
	out_position = in_position;
}