#version 450

layout (location = 0) in vec2 in_UV;

layout (location = 0) out vec4 outColor;

layout (set = 0, binding = 0) uniform sampler2D albedo_tex;
layout (set = 0, binding = 1) uniform sampler2D normal_tex;
layout (set = 0, binding = 2) uniform sampler2D position_tex;

layout (set = 1, binding = 0) uniform sampler2D shadow_tex;

#define SPECULAR_STRENGH 0.5

layout (push_constant) uniform Push{
	vec3 cameraPosition;
	vec3 cameraDirection;
	vec3 color;
	vec3 position;
	vec3 direction;
	float maxAngle;
	float intensity;
} light;

float getRadialFalloff(in vec3 position){
	float d = distance(position, light.position);
	return light.intensity / pow(d, 2);
}

float getAngularFalloff(in vec3 position){
	float product = dot(light.direction, light.position - position);
	return smoothstep(0., light.maxAngle, product);
}

float getNormalFalloff(in vec3 position, in vec3 normal){
	vec3 directionToLight = light.position - position;
	return clamp(dot(directionToLight, normal), 0., 1.);
}

float getSpecular(in vec3 position, in vec3 normal){
	vec3 viewDir = normalize(light.cameraPosition - position);
	vec3 reflectDir = reflect(-light.direction, normal);  
	return pow(max(dot(light.cameraDirection, reflectDir), 0.0), 32);
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

	float radialFalloff = getRadialFalloff(position);
	float angularFalloff = getAngularFalloff(position);
	float normalFalloff = getNormalFalloff(position, normal);
	float spec = getSpecular(position, normal);

	float coef = (radialFalloff * angularFalloff * normalFalloff) + spec;
	outColor = vec4(albedo, 1.) * coef;
}