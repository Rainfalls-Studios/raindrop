#ifndef __RAINDROP_GRAPHICS_RENDERER_HPP__
#define __RAINDROP_GRAPHICS_RENDERER_HPP__

#include "common.hpp"
#include "Materials/Manager.hpp"

#include "Wrappers/Material.hpp"

namespace Raindrop::Graphics{
	using namespace Wrappers;
	using Material = Materials::Material;

	class Renderer{
		public:
			Renderer(::Raindrop::Context& context);
			~Renderer();

			// === Materials ===

			MaterialWrapper createMaterial();
			MaterialWrapper registerMaterial(const Material& material);
			void unregisterMaterial(const MaterialWrapper& wrapper);

			Materials::Manager& materials();
			const Materials::Manager& materials() const;


			// Runtime

			void render();
			void events();

			Context& context();

		private:
			Context* _context;

			VkCommandBuffer beginFrame();
			void endFrame();

			void createRenderCommandPool();
			void allocateFrameCommandBuffers();

			void freeFrameCommandBuffers();
			void destroyRenderCommandPool();

			VkCommandPool _renderCommandPool;
			std::vector<VkCommandBuffer> _frameCommandBuffers;
			std::size_t _currentFrameID;
	};
}

#endif