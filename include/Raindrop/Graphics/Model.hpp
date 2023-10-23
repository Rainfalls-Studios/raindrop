#ifndef __RAINDROP_GRAPHICS_MODEL_HPP__
#define __RAINDROP_GRAPHICS_MODEL_HPP__

#include <Raindrop/Graphics/common.hpp>
#include <Raindrop/Graphics/builders/ModelBuilder.hpp>
#include <Raindrop/Graphics/Mesh/common.hpp>

namespace Raindrop::Graphics{
	class Model : public Core::Asset::Asset{
		public:
			Model(GraphicsContext& context, const Builders::ModelBuilder& builder);
			~Model();

			void draw(const FrameState& frameState, VkBuffer* additionalVertexBuffers = nullptr, uint32_t vertexBufferCount = 0);

		public:
			GraphicsContext& _context;

			std::vector<std::unique_ptr<Mesh::Mesh>> _meshes;
	};
}

#endif