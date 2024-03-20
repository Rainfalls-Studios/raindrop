#ifndef __RAINDROP_RENDERER_MODEL_MODEL_HPP__
#define __RAINDROP_RENDERER_MODEL_MODEL_HPP__

#include "common.hpp"
#include <Raindrop/Core/Asset/Asset.hpp>

namespace Raindrop::Renderer::Model{
	class Model : public ::Raindrop::Core::Asset::Asset{
		public:
			Model(Context& context, const Path& path);
			virtual ~Model() override;

			void render(VkCommandBuffer commandBuffer);

		private:
			Context& _context;
			std::vector<std::unique_ptr<Mesh>> _meshes;
			
	};
}

#endif