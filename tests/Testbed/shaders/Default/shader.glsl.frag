#version 450

layout (location = 0) in vec3 in_color;
layout (location = 1) in vec3 in_position;
layout (location = 2) in vec3 in_normal;
layout (location = 3) in vec2 in_UV;

layout (location = 0) out vec4 out_color;

void main(){
	vec3 sunDirection = vec3(0., -1., 0.);

    vec3 normal = normalize(in_normal);
    float diffuseFactor = max(dot(normal, -sunDirection), 0.0);

	out_color = vec4(normal, 1.) * diffuseFactor;
	// out_color = vec4(1.)1
}