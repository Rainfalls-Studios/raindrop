#ifndef __RAINDROP_GRAPHICS_RENDER_SYSTEMS_RENDER_SYSTEM_REGISTRY_HPP__
#define __RAINDROP_GRAPHICS_RENDER_SYSTEMS_RENDER_SYSTEM_REGISTRY_HPP__

#include "common.hpp"
#include "../../Utils/Registry.hpp"

namespace Raindrop::Graphics::RenderSystems{
	class RenderSystemRegistry : private Utils::Registry<std::unique_ptr<RenderSystem>, RenderSystemID>{
		public:
			RenderSystemRegistry(Context& context);
			~RenderSystemRegistry();

			RenderSystemID create(std::unique_ptr<RenderSystem>&& system);

			template<typename T, typename... Args>
			RenderSystemID create(Args&&... args){
				static_assert(std::is_base_of_v<RenderSystem, T>, "The render system has to be derived out of Raindrop::Graphics::RenderSystems::RenderSystem class");
				return create(std::make_unique<T>(args...));
			}

			void destroy(const RenderSystemID& ID);

			RenderSystem* get(const RenderSystemID& ID);

			using Registry::exists;
			using Registry::inRange;

			using Registry::reservedSize;
			using Registry::freeSize;
			using Registry::usedSize;

		private:
			Context& _context;

	};
}

#endif