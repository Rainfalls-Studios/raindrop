#version 450 core
#pragma shader_stage(vertex)

layout (location = 0) out vec2 frag_UV;

struct Vertex{
	vec2 position;
	vec2 UV;
};

Vertex vertices[6] = {
	{{-1., -1.}, {0., 0.}},
	{{ 1., -1.}, {1., 0.}},
	{{-1.,  1.}, {0., 1.}},
	{{-1.,  1.}, {0., 1.}},
	{{ 1., -1.}, {1., 0.}},
	{{ 1.,  1.}, {1., 1.}}
};

void main(){
	frag_UV = vertices[gl_VertexIndex].UV;
	gl_Position = vec4(vertices[gl_VertexIndex].position, 0., 1.);
}