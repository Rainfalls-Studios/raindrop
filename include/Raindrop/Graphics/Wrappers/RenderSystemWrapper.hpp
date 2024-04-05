#ifndef __RAINDROP_GRAPHICS_WRAPPERS_RENDER_SYSTEM_HPP__
#define __RAINDROP_GRAPHICS_WRAPPERS_RENDER_SYSTEM_HPP__

#include "common.hpp"
#include "../RenderSystems/common.hpp"
#include "../../Utils/Wrapper.hpp"

namespace Raindrop::Graphics::Wrappers{
	RenderSystems::RenderSystem& ___get_render_system___(Context& context, const RenderSystems::RenderSystemID& ID);

	template<typename T = RenderSystems::RenderSystem>
	class RenderSystemWrapper : private Utils::Wrapper<Context, T, RenderSystems::RenderSystemID, RenderSystems::INVALID_RENDER_SYSTEM_ID>{
		public:
			using RenderSystemID = RenderSystems::RenderSystemID;
			using RenderSystem = RenderSystems::RenderSystem;

			static_assert(std::is_base_of_v<RenderSystem, T>, "Cannot create a render system wrapper from a class that is not derived from Raindrop::Graphics::RenderSystem (Raindrop::Graphics::RenderSystems::RenderSystem)");

			using Wrapper = Utils::Wrapper<Context, T, RenderSystems::RenderSystemID, RenderSystems::INVALID_RENDER_SYSTEM_ID>;
            using Wrapper::Wrapper;
            using Wrapper::operator=;
            using Wrapper::operator->;
            using Wrapper::ID;

			virtual T& get() final override{
				return static_cast<T&>(___get_render_system___(*Wrapper::_context, Wrapper::_ID));
			}

			virtual const T& get() const final override{
				return static_cast<const T&>(___get_render_system___(*Wrapper::_context, Wrapper::_ID));
			}
	};
}

#endif