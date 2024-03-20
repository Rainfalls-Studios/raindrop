#version 450

layout (location = 0) in vec3 in_position;
layout (location = 1) in vec3 in_normal;
layout (location = 2) in vec3 in_color;
layout (location = 3) in vec2 in_UV;

layout (location = 0) out vec4 out_color;

void main(){
	vec3 sunDirection = vec3(0., -0.52532198881, 0.85090352453);

	out_color = vec4(in_color, 0.) * dot(in_normal, -sunDirection);
	// out_color = vec4(1.);
}