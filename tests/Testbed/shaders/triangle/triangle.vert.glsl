#version 450 core
#pragma shader_stage(vertex)

layout (location = 0) in vec3 vert_position;
layout (location = 1) in vec3 vert_colors;

layout (location = 0) out vec3 frag_color;
layout (location = 1) out vec2 frag_UV;
layout (location = 2) out vec3 frag_normal;


layout(push_constant, std430) uniform pc {
    mat4 modelTransform;
	mat4 cameraViewProjection;
} pushConstant;


void main(){
	frag_UV = vec2(0.f);
	frag_normal = vert_colors;
	frag_color = vec3(1.f);

	vec4 position = vec4(vert_position, 1.f) * pushConstant.modelTransform;

	gl_Position = position;
}