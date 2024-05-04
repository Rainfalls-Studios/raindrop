#ifndef __RAINDROP_GRAPHICS_RENDER_SYSTEMS_RENDER_SYSTEM_HPP__
#define __RAINDROP_GRAPHICS_RENDER_SYSTEMS_RENDER_SYSTEM_HPP__

#include "common.hpp"

namespace Raindrop::Graphics::RenderSystems{
	class RenderSystem{
		public:
			RenderSystem() = default;
			virtual ~RenderSystem() = default;

			virtual std::string getName() const;
			virtual std::string getDescription() const;
			virtual Version getVersion() const;

			virtual void render(VkCommandBuffer commandBuffer, ::Raindrop::Wrappers::SceneWrapper scene) = 0;

	};
}

#endif