#ifndef __RAINDROP_GRAPHICS_RENDER_SYSTEMS_REGISTRY_HPP__
#define __RAINDROP_GRAPHICS_RENDER_SYSTEMS_REGISTRY_HPP__

#include "common.hpp"

namespace Raindrop::Graphics::RenderSystems{
	class Registry{
		public:
			Registry(Context& context);
			~Registry();

			template<typename T, typename... Args>
			RenderSystemID createRenderSystem(Args&&... args){
				static_assert(std::is_base_of_v<RenderSystem, T>, "Cannot create a render system from a class that is not derived from Raindrop::Graphics::RenderSystem");
				std::unique_ptr<T> system = std::make_unique<T>(args...);
				return registerRenderSystem(std::move(system));
			}

			RenderSystemID registerRenderSystem(std::unique_ptr<RenderSystem> system);
			void destroyRenderSystem(const RenderSystemID& ID);

			RenderSystem& getRenderSystem(const RenderSystemID& ID);

		private:
			Context& _context;
			std::vector<std::unique_ptr<RenderSystem>> _renderSystems;
			std::queue<RenderSystemID> _freeIDs;

	};
}

#endif