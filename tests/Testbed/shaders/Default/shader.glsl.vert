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

vec3 positions[] = {
	vec3(0., 0., 0.),
	vec3(1., 0., 0.),
	vec3(0., 1., 0.),
};

void main(){
	out_color = in_color;
	out_normal = in_normal;
	out_position = in_position;
	out_UV = in_UV;

	// vec3 position = positions[gl_VertexIndex];

	gl_Position = Matrix.viewProjection * Matrix.localTransform * vec4(in_position, 1.);
}