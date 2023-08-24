#version 450

layout (location = 0) in vec2 in_UV;

layout (location = 0) out vec4 outColor;

layout (set = 0, binding = 0) uniform sampler2D albedo_tex;
layout (set = 0, binding = 1) uniform sampler2D normal_tex;
layout (set = 0, binding = 2) uniform sampler2D position_tex;

layout (set = 1, binding = 0) uniform sampler2D shadow_tex;

#define SPECULAR_STRENGH 0.5

layout (push_constant) uniform Push{
	mat4 lightMatrix;
	vec3 cameraPosition;
	vec3 cameraDirection;
	vec3 color;
	vec3 direction;
	float intensity;
} light;

float getNormalFalloff(in vec3 position, in vec3 normal){
	vec3 directionToLight = -light.direction;
	return clamp(dot(directionToLight, normal), 0., 1.);
}

float getSpecular(in vec3 position, in vec3 normal){
	vec3 viewDir = normalize(light.cameraPosition - position);
	vec3 reflectDir = reflect(light.direction, normal);  
	return pow(max(dot(viewDir, reflectDir), 0.0), 32);
}

float getDiffuse(in vec3 position, in vec3 normal){
	vec3 directionToLight = -light.direction;
	return max(dot(normal, directionToLight), 0.0);
}

float ShadowCalculation(vec4 fragPosLightSpace){
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;
    float closestDepth = texture(shadow_tex, projCoords.xy).r; 
    float currentDepth = projCoords.z;
    float shadow = closestDepth < currentDepth  ? 0.0 : 1.0;

	// vec4 color = projCoords.x > 0. && projCoords.x < 1. && projCoords.y > 0. && projCoords.y < 1. ? vec4(1.) : vec4(0.);

	if (projCoords.x < 0.5){
    	return shadow;
	}

	if (projCoords.x > 0.5 && projCoords.y < 0.5){
		return closestDepth;
	}

	if (projCoords.x > 0.5 && projCoords.y > 0.5){
		return currentDepth;
	}
}  

void main(){

	// query samples
	vec3 position = texture(position_tex, in_UV).rgb;
	vec3 normal = texture(normal_tex, in_UV).rgb;
	vec3 albedo = vec3(1., 1., 1.);
	float specular = 0.;

	{
		vec4 albedo_sample = texture(albedo_tex, in_UV);
		albedo = albedo_sample.rgb;
		specular = albedo_sample.a;
	}

	float normalFalloff = getNormalFalloff(position, normal);
	float spec = getSpecular(position, normal);
	float diff = getDiffuse(position, normal);
	float shadow = ShadowCalculation(light.lightMatrix * vec4(position, 1.0));

	float coef = normalFalloff * (spec + diff);
	outColor = vec4(albedo * light.color, 1.) * coef * light.intensity * shadow;
}