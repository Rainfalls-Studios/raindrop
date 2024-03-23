#ifndef __RAINDROP_RENDERER_MODEL_MESH_HPP__
#define __RAINDROP_RENDERER_MODEL_MESH_HPP__

#include "common.hpp"

namespace Raindrop::Renderer::Model{
	class Mesh{
		public:
			Mesh(Context& context);
			~Mesh();

			VkBuffer& vertexBuffer();
			VkDeviceMemory& vertexMemory();

			VkBuffer& indexBuffer();
			VkDeviceMemory& indexMemory();

			void allocateVertexBuffer(const VkDeviceSize& size, const std::size_t& count);
			void allocateIndexBuffer(const VkDeviceSize& size, const std::size_t& count);

			void render(VkCommandBuffer commandBuffer);

			VkDescriptorSet& descriptorSet();

		private:
			Context& _context;

			VkBuffer _vertexBuffer;
			VkDeviceMemory _vertexMemory;

			VkBuffer _indexBuffer;
			VkDeviceMemory _indexMemory;

			std::size_t _vertexCount;
			std::size_t _indexCount;

			// handled by the model
			VkDescriptorSet _descriptorSet;
	};
}

#endif