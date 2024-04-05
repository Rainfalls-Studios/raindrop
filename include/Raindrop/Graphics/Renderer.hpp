#ifndef __RAINDROP_GRAPHICS_RENDERER_HPP__
#define __RAINDROP_GRAPHICS_RENDERER_HPP__

#include "common.hpp"
#include "Materials/Manager.hpp"
#include "RenderSystems/Registry.hpp"

#include "Wrappers/MaterialWrapper.hpp"
#include "Wrappers/RenderSystemWrapper.hpp"

namespace Raindrop::Graphics{
	using namespace Wrappers;
	using Material = Materials::Material;
	using RenderSystem = RenderSystems::RenderSystem;

	class Renderer{
		public:
			Renderer(::Raindrop::Context& context);
			~Renderer();

			// === Materials ===

			MaterialWrapper createMaterial();
			MaterialWrapper registerMaterial(const Material& material);
			void destroyMaterial(const MaterialWrapper& wrapper);

			Materials::Manager& materials();
			const Materials::Manager& materials() const;

			// === Render systems ===

			template<typename T, typename... Args>
			RenderSystemWrapper<T> createRenderSystem(Args&&... args){
				auto ID = renderSystems().createRenderSystem<T>(args...);
				return RenderSystemWrapper(*_context, ID);
			}

			RenderSystemWrapper<> registerRenderSystem(std::unique_ptr<RenderSystem> system);
			void destroyRenderSystem(const RenderSystemWrapper<>& wrapper);

			RenderSystems::Registry& renderSystems();
			const RenderSystems::Registry& renderSystems() const;

			// === Runtime ===

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