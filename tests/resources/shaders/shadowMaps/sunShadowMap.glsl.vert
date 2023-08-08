#version 450

layout (location = 0) in vec3 in_position;

layout (push_constant) uniform Push{
	mat4 lightSpaceMatrix;
	mat4 modelMatrix;
} push;

void main(){
	gl_Position = push.lightSpaceMatrix * push.modelMatrix * vec4(in_position, 1.0);
}