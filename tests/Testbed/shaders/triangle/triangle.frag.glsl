#version 450 core
#pragma shader_stage(fragment)

layout (location = 0) in vec2 frag_UV;

layout (location = 0) out vec4 out_color;

void main(){
	out_color = vec4(frag_UV, 0.f, 0.f);
}