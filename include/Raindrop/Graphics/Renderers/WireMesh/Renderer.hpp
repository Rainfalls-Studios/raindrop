#ifndef __RAINDROP_GRAPHICS_BASE_RENDER_WIRE_MESH_RENDERER_HPP__
#define __RAINDROP_GRAPHICS_BASE_RENDER_WIRE_MESH_RENDERER_HPP__

#include <Raindrop/Graphics/BaseRender/WireMesh/common.hpp>

namespace Raindrop::Graphics::BaseRender::WireMesh{
	class Renderer : public BaseRenderer{
		public:
			Renderer(GraphicsContext& context);
			virtual ~Renderer() override;
			
			virtual void begin(SceneFramebuffer& target) override;
			virtual void end(SceneFramebuffer& target) override;
			
			virtual void render(Entity parent) override;

		private:
			GraphicsContext& _context;

			void loadPipeline();

			std::weak_ptr<GraphicsPipeline> _pipeline;
	};
}

#endif