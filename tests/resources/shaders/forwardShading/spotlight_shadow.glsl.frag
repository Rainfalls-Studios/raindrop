#version 450

layout (location = 0) in vec2 in_UV;

layout (location = 0) out vec4 outColor;

layout (set = 0, binding = 0) uniform sampler2D albedo_tex;
layout (set = 0, binding = 1) uniform sampler2D normal_tex;
layout (set = 0, binding = 2) uniform sampler2D position_tex;

layout (set = 1, binding = 0) uniform sampler2D shadow_tex;

#define SPECULAR_STRENGH 0.5

struct Vec3{
	float x;
	float y;
	float z;
};

vec3 toVec3(in Vec3 vec){
	return vec3(vec.x, vec.y, vec.z);
}

vec3 uintToVec3(in uint value) {
    float byteMax = 255.0;

    float byte0 = float(value & 0xFFu) / byteMax;
    float byte1 = float((value >> 8) & 0xFFu) / byteMax;
    float byte2 = float((value >> 16) & 0xFFu) / byteMax;

    return vec3(byte0, byte1, byte2);
}

layout (push_constant) uniform Push{
	mat4 lightMatrix;
	Vec3 cameraPosition;
	Vec3 cameraDirection;
	Vec3 position;
	Vec3 direction;
	uint color;
	float outerCutoff;
	float innerCutoff;
	float intensity;
} light;

float getRadialFalloff(in vec3 position){
	vec3 directionToLight = toVec3(light.position) - position;
	return light.intensity / dot(directionToLight, directionToLight);
}

float getAngularFalloff(in vec3 position){
	vec3 lightVector = normalize(toVec3(light.position) - position);
	float cosTheta = dot(toVec3(light.direction), -lightVector);
	float theta = acos(cosTheta);
	return smoothstep(light.outerCutoff, light.innerCutoff, theta);
}

float getNormalFalloff(in vec3 position, in vec3 normal){
	vec3 directionToLight = toVec3(light.position) - position;
	return max(dot(normalize(normal), normalize(directionToLight)), 0.);
}

float getSpecular(in vec3 position, in vec3 normal){
	vec3 viewDir = normalize(toVec3(light.cameraPosition) - position);
	vec3 reflectDir = reflect(normalize(position - toVec3(light.position)), normal);  
	return pow(max(dot(viewDir, reflectDir), 0.0), 32);
}

float getDiffuse(in vec3 position, in vec3 normal){
	vec3 lightDir = normalize(toVec3(light.position) - position);  
	return max(dot(normal, lightDir), 0.0);
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
	float diff = getDiffuse(position, normal);

	float coef = radialFalloff * normalFalloff * angularFalloff * (spec + diff);
	outColor = vec4(albedo * uintToVec3(light.color), 1.) * coef;
}