#ifndef __RAINDROP_GRAPHICS_TYPES_HPP__
#define __RAINDROP_GRAPHICS_TYPES_HPP__

#include <cstddef>

namespace Raindrop::Graphics{
	static constexpr std::size_t WHOLE_SIZE = static_cast<std::size_t>(~0ULL);
	
	struct Context;
	class Buffer;
	class CommandBuffer;
	class CommandPool;
	class ComputePipeline;
	class DescriptorPool;
	class DescriptorSet;
	class GraphicsPipeline;
	class Image;
	class ImageView;
	class PipelineLayout;
	class RenderPass;
	class ShaderModule;
	class Formats;
	class DescriptorSetLayout;
	class Memory;
	class Mesh;
	class MeshData;
	class VertexLayout;
	class Sampler;
}

#endif