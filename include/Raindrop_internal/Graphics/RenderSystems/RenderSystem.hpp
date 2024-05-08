#ifndef __RAINDROP_INTERNAL_GRAPHICS_RENDER_SYSTEMS_RENDER_SYSTEM_HPP__
#define __RAINDROP_INTERNAL_GRAPHICS_RENDER_SYSTEMS_RENDER_SYSTEM_HPP__

#include "common.hpp"

namespace Raindrop::Internal::Graphics::RenderSystems{
	class RenderSystem{
		public:
			RenderSystem() = default;
			virtual ~RenderSystem() = default;

			virtual std::string getName() const;
			virtual std::string getDescription() const;

			virtual void render(VkCommandBuffer commandBuffer, ::Raindrop::Scene& scene) = 0;

	};
}

#endif