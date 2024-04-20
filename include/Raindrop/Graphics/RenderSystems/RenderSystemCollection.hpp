#ifndef __RAINDROP_GRAPHICS_RENDER_SYSTEMS_RENDER_SYSTEM_COLLECTION_HPP__
#define __RAINDROP_GRAPHICS_RENDER_SYSTEMS_RENDER_SYSTEM_COLLECTION_HPP__

#include "common.hpp"
#include <Raindrop/Wrappers/SceneWrapper.hpp>

namespace Raindrop::Graphics::RenderSystems{
	class RenderSystemCollection{
		public:
			RenderSystemCollection(Context& context);
			~RenderSystemCollection();

			void pushRenderSystem(const std::shared_ptr<RenderSystem>& system);
			void pushRenderSystems(const std::initializer_list<std::shared_ptr<RenderSystem>>& systems);
			void render(VkCommandBuffer commandBuffer, ::Raindrop::Wrappers::SceneWrapper scene);

		private:
			Context& _context;
			std::list<std::shared_ptr<RenderSystem>> _renderSystems;
	};
}

#endif