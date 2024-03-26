#version 450

layout (location = 0) in vec3 in_color;
layout (location = 1) in vec3 in_position;
layout (location = 2) in vec3 in_normal;
layout (location = 3) in vec2 in_UV;

layout (set = 0, binding = 0) uniform Material{
	vec3 diffuse;
	vec3 specular;
	float shininess;
} material;

layout (set = 0, binding = 1) uniform sampler2D diffuseTexture;

layout (location = 0) out vec4 out_color;

layout (push_constant) uniform matrix{
	mat4 viewProjection;
	mat4 localTransform;
} Matrix;

void main(){
	const vec3 sunDirection = vec3(0.866, -0.5, 0.);

    const vec3 normal = vec3(Matrix.localTransform * vec4(normalize(in_normal), 1.0));
    const float diffuseFactor = max(dot(normal, -sunDirection), 0.0) + 0.1;

	out_color = (vec4(1.) * diffuseFactor) * texture(diffuseTexture, in_UV);
	// out_color = vec4(1.);
}