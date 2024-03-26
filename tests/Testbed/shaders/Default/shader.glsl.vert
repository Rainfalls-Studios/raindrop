#version 450

layout (location = 0) in vec3 in_position;
layout (location = 1) in vec3 in_normal;
layout (location = 2) in vec3 in_color;
layout (location = 3) in vec2 in_UV;

layout (location = 0) out vec3 out_color;
layout (location = 1) out vec3 out_position;
layout (location = 2) out vec3 out_normal;
layout (location = 3) out vec2 out_UV;

layout (push_constant) uniform matrix{
	mat4 viewProjection;
	mat4 localTransform;
} Matrix;

void main(){
	vec4 position = Matrix.viewProjection * Matrix.localTransform * vec4(in_position, 1.);

	out_normal = mat3(Matrix.localTransform) * in_normal;
	out_position = vec3(position);
	out_color = in_color;
	out_UV = in_UV;

	gl_Position = position;
}