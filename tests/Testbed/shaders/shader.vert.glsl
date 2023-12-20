#version 450
#pragma shader_stage(vertex)

struct Vertex{
	vec3 position;
	vec3 color;
};

Vertex vertices[] = {
	Vertex(vec3(-1.0, -1.0, 0.0), vec3(1.0, 1.0, 1.0)),
	Vertex(vec3( 1.0, -1.0, 0.0), vec3(1.0, 1.0, 1.0)),
	Vertex(vec3( 1.0,  1.0, 0.0), vec3(1.0, 1.0, 1.0)),

	Vertex(vec3(-1.0, -1.0, 0.0), vec3(1.0, 1.0, 1.0)),
	Vertex(vec3(-1.0,  1.0, 0.0), vec3(1.0, 1.0, 1.0)),
	Vertex(vec3( 1.0,  1.0, 0.0), vec3(1.0, 1.0, 1.0))
};

layout (location = 0) out vec3 out_color;
layout (location = 1) out vec3 out_position;

void main(){
	vec3 position = vertices[gl_VertexIndex].position;

	out_color = vertices[gl_VertexIndex].color;
	out_position = position;

	gl_Position = vec4(position, 1.0);
}