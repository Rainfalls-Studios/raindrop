#ifndef __RAINDROP_GRAPHICS_BUFFERS_ENUMS_HPP__
#define __RAINDROP_GRAPHICS_BUFFERS_ENUMS_HPP__

namespace Raindrop::Graphics::Buffers{
	enum VertexAttributeType{
		Position,
		Normal,
		Uv,
		Color,
		Tangent,
		Bitangent,
		BoneWeight,
		BoneIndex,
		Custom,
		None,
	};
}

#endif