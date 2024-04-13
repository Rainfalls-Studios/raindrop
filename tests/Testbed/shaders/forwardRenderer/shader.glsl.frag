#version 450

#define SUN_DIRECTION vec3(0.866, 0.5, 0.)
#define SUN_COLOR vec3(1.)

layout(location = 0) in vec3 in_color;
layout(location = 1) in vec3 in_position;
layout(location = 2) in vec3 in_normal;
layout(location = 3) in vec2 in_UV;

layout(set = 0, binding = 0) uniform Material {
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	float shininess;
	float specularFactor;
} material;

layout(set = 0, binding = 1) uniform sampler2D diffuseTexture;

layout(set = 1, binding = 0) uniform Scene{
	vec4 ambientColor;
	mat4 viewProjection;
	vec4 cameraPosition;
} scene;

layout(location = 0) out vec4 out_color;

void main() {
	// process input
	vec3 norm = normalize(in_normal);
	vec3 lightDir = normalize(SUN_DIRECTION);

	// ambient
	vec3 ambient = scene.ambientColor.xyz + material.ambient.xyz;

	// diffuse
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * SUN_COLOR;

	// specular
	vec3 viewDir = normalize(scene.cameraPosition.xyz - in_position);
	vec3 reflectDir = reflect(-viewDir, norm);

	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = material.specular.xyz * spec * material.specularFactor * SUN_COLOR;

	// object color
	vec3 objectColor = texture(diffuseTexture, in_UV).xyz;

	// final
	vec3 result = (ambient + diffuse + specular) * objectColor;
    out_color = vec4(result, 1.0);
}