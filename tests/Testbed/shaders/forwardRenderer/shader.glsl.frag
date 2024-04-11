#version 450

#define SUN_DIRECTION vec3(0.866, -0.5, 0.)
#define SCENE_AMBIENT_COLOR vec3(0.7, 0.7, 0.7)

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

layout(set = 1, binding = 0) uniform Scene{
	mat4 viewProjection;
	vec4 ambientColor;
} scene;

layout(location = 0) out vec4 out_color;

void main() {
    vec3 normal = in_normal;

    vec3 ambientColor = SCENE_AMBIENT_COLOR + material.ambient.xyz;

    float diffuseFactor = max(dot(normal, -SUN_DIRECTION), 0.0);
    vec4 diffuseColor = texture(diffuseTexture, in_UV) * vec4(material.diffuse.xyz * diffuseFactor, 1.);

    vec4 result = vec4(ambientColor, 1.) * diffuseColor;

    // Output
    out_color = result;
}