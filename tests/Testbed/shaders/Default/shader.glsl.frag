#version 450

#define SUN_DIRECTION vec3(0.866, -0.5, 0.)

layout (location = 0) in vec3 in_color;
layout (location = 1) in vec3 in_position;
layout (location = 2) in vec3 in_normal;
layout (location = 3) in vec2 in_UV;


layout (set = 0, binding = 0) uniform Material{
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	float shininess;
} material;

layout (set = 0, binding = 1) uniform sampler2D diffuseTexture;

layout (push_constant) uniform Matrix{
	mat4 viewProjection;
	mat4 localTransform;
} matrix;


layout (location = 0) out vec4 out_color;


void main(){
	vec3 normal = normalize(in_normal);

	// light calculations
	vec3 ambient = (material.ambient.xyz + vec3(0.5));

	float diff = max(dot(normal, -SUN_DIRECTION), 0.0);
	vec3 diffuse = (diff * material.diffuse.xyz) * texture(diffuseTexture, in_UV).xyz;

	vec3 result = ambient * diffuse;

	// output
	out_color = vec4(result, 1.0);
}