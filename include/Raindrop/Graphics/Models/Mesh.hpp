#ifndef __RAINDROP_GRAPHICS_MODELS_MESH_HPP__
#define __RAINDROP_GRAPHICS_MODELS_MESH_HPP__

#include "common.hpp"
#include "../Materials/common.hpp"

namespace Raindrop::Graphics::Models{
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

			Materials::MaterialID& materialID();

		private:
			Context& _context;

			VkBuffer _vertexBuffer;
			VkDeviceMemory _vertexMemory;

			VkBuffer _indexBuffer;
			VkDeviceMemory _indexMemory;

			std::size_t _vertexCount;
			std::size_t _indexCount;

			Materials::MaterialID _material;
	};
}

#endif