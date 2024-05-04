#ifndef __RAINDROP_GRAPHICS_RENDER_SYSTEMS_RENDER_SEQUENCE_HPP__
#define __RAINDROP_GRAPHICS_RENDER_SYSTEMS_RENDER_SEQUENCE_HPP__

#include "common.hpp"

namespace Raindrop::Graphics::RenderSystems{
	class RenderSequence{
		public:
			RenderSequence(const std::initializer_list<std::shared_ptr<RenderSystem>>& systems = {});
			~RenderSequence();

			void render(VkCommandBuffer commandBuffer, ::Raindrop::Wrappers::SceneWrapper scene);

			const std::list<std::shared_ptr<RenderSystem>>& renderSystems() const;
			std::list<std::shared_ptr<RenderSystem>>& renderSystems();

		private:
			std::list<std::shared_ptr<RenderSystem>> _renderSystems;
	};
}

#endif