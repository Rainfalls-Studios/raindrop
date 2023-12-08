#ifndef __RAINDROP_GRAPHICS_RENDER_PASS_RENDER_PASS_MANAGER_HPP__
#define __RAINDROP_GRAPHICS_RENDER_PASS_RENDER_PASS_MANAGER_HPP__

#include <Raindrop/Graphics/RenderPass/common.hpp>

namespace Raindrop::Graphics::RenderPass{
	class RenderPassManager{
		public:
			RenderPassManager(Context& context);
			~RenderPassManager();

			std::shared_ptr<RenderPass> create(const RenderPassBuilder& builder);
			std::shared_ptr<RenderPass> get(const std::string& name);

			void clean();
		
		private:
			Context& _context;

			std::unordered_map<std::string, std::shared_ptr<RenderPass>> _renderPasses;
	};
}

#endif