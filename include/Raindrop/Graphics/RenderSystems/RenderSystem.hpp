#ifndef __RAINDROP_GRAPHICS_RENDER_SYSTEMS_RENDER_SYSTEM_HPP__
#define __RAINDROP_GRAPHICS_RENDER_SYSTEMS_RENDER_SYSTEM_HPP__

#include "common.hpp"
#include <Raindrop/Wrappers/SceneWrapper.hpp>

namespace Raindrop::Graphics::RenderSystems{
	class RenderSystem{
		public:
			RenderSystem() = default;
			virtual ~RenderSystem() = default;

			virtual std::string name() const;
			virtual std::string description() const;
			virtual Version version() const;

			virtual void render(VkCommandBuffer commandBuffer, ::Raindrop::Wrappers::SceneWrapper scene) = 0;

	};
}

#endif