#version 450

layout (location = 0) in vec2 in_UV;

layout (location = 0) out vec4 outColor;

layout (set = 0, binding = 0) uniform sampler2D tex;

void main(){
	outColor = vec4(texture(tex, in_UV).rgb, 1.);
}