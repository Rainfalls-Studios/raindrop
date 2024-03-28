#version 450

#define SUN_DIRECTION vec3(0.866, -0.5, 0.)
#define SCENE_AMBIENT_COLOR vec3(0.7)

layout(location = 0) in vec3 in_color;
layout(location = 1) in vec3 in_position;
layout(location = 2) in vec3 in_normal;
layout(location = 3) in vec2 in_UV;

layout(set = 0, binding = 0) uniform Material {
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	float shininess;
} material;

layout(set = 0, binding = 1) uniform sampler2D diffuseTexture;

layout(push_constant) uniform Matrix {
	mat4 viewProjection;
	mat4 localTransform;
} matrix;

layout(location = 0) out vec4 out_color;

void main() {
	vec3 normal = in_normal;//normalize(in_normal);

	// Ambient lighting
	vec3 ambientColor = SCENE_AMBIENT_COLOR + material.ambient.xyz;

	// Diffuse lighting
	float diffuseFactor = max(dot(normal, -SUN_DIRECTION), 0.0);
	vec3 diffuseColor = texture(diffuseTexture, in_UV).xyz * material.diffuse.xyz;

	// Final color (ambient + diffuse)
	vec3 result = (ambientColor + diffuseFactor) * diffuseColor;

	// Output
	out_color = vec4(result, 1.0);
}