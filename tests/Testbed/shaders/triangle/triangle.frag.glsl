#version 450 core
#pragma shader_stage(fragment)

layout (location = 0) in vec3 frag_color;
layout (location = 1) in vec2 frag_UV;
layout (location = 2) in vec3 frag_normal;

layout (location = 0) out vec4 out_color;

void main(){
	out_color = vec4(frag_normal, 0.f);
}