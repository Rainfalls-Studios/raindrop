#version 450

layout (location = 0) out vec4 out_color;

struct Vertex{
	vec2 position;
	vec4 color;
};

Vertex vertices[3] = {
	{{.0,  -0.5}, {1., 0., 0., 1.}},
	{{-0.5, 0.3}, {0., 1., 0., 1.}},
	{{0.5,  0.3}, {0., 0., 1., 1.}}
};

void main(){
	out_color = vertices[gl_VertexIndex].color;
	gl_Position = vec4(vertices[gl_VertexIndex].position, 0., 1.);
}