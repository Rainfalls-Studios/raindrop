#version 450

layout (location = 0) out vec2 out_UV;

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
	out_UV = vertices[gl_VertexIndex].UV;
	gl_Position = vec4(vertices[gl_VertexIndex].position, 0., 1.);
}