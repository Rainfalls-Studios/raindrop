#ifndef __RAINDROP_INTERNAL_GRAPHICS_RENDER_SYSTEMS_RENDER_SYSTEM_REGISTRY_HPP__
#define __RAINDROP_INTERNAL_GRAPHICS_RENDER_SYSTEMS_RENDER_SYSTEM_REGISTRY_HPP__

#include "common.hpp"
#include "../../Templates/Registry.hpp"

namespace Raindrop::Internal::Graphics::RenderSystems{
	class RenderSystemRegistry : Templates::Registry<std::unique_ptr<RenderSystem>, RenderSystemID>{
		public:
			RenderSystemRegistry(Context& context);
			~RenderSystemRegistry();

			RenderSystemID create(std::unique_ptr<RenderSystem>&& system);

			template<typename T, typename... Args>
			RenderSystemID create(Args&&... args){
				static_assert(std::is_base_of_v<RenderSystem, T>, "The render system has to be derived out of Raindrop::Internal::Graphics::RenderSystems::RenderSystem class");
				return create(std::make_unique<T>(args...));
			}

			void destroy(const RenderSystemID& ID);

			RenderSystem* get(const RenderSystemID& ID);

		private:
			Context& _context;

	};
}

#endif