#version 450
#pragma shader_stage(vertex)

struct Vertex{
	vec2 position;
	vec2 UV;
};

Vertex verticies[] = {
	{vec2(-1.0, -1.0), vec2(0.0, 0.0)},
	{vec2( 1.0, -1.0), vec2(1.0, 0.0)},
	{vec2( 1.0,  1.0), vec2(1.0, 1.0)},

	{vec2(-1.0, -1.0), vec2(0.0, 0.0)},
	{vec2(-1.0,  1.0), vec2(0.0, 1.0)},
	{vec2( 1.0,  1.0), vec2(1.0, 1.0)},
};

layout (location = 0) out vec2 out_UV;

void main(){
	out_UV = verticies[gl_VertexID].UV;
	gl_Position = vec4(verticies[gl_VertexID].position, 0.0, 1.0);
}