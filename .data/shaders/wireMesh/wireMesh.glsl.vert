#version 450

layout (location = 0) in vec3 in_position;

layout (push_constant) uniform Push{
	mat4 projectionViewMatrix;
	mat4 modelMatrix;
} push;

void main(){
	vec4 positionWorld = push.modelMatrix * vec4(in_position, 1.0);
	gl_Position = push.projectionViewMatrix * positionWorld;
}