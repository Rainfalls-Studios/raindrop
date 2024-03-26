#version 450

layout (location = 0) in vec2 frag_UV;

layout (location = 0) out vec4 out_color;

layout(binding = 0) uniform sampler2D texSampler;


void main(){
    out_color = texture(texSampler, frag_UV);
}