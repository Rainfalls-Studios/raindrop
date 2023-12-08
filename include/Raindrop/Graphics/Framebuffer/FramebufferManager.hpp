#ifndef __RAINDROP_GRAPHICS_FRAMEBUFFER_FRAMEBUFFER_MANAGER_HPP__
#define __RAINDROP_GRAPHICS_FRAMEBUFFER_FRAMEBUFFER_MANAGER_HPP__

#include <Raindrop/Graphics/Framebuffer/common.hpp>

namespace Raindrop::Graphics::Framebuffer{
	class FramebufferManager{
		public:
			FramebufferManager(Context& context);
			~FramebufferManager();

			std::shared_ptr<Framebuffer> create(FramebufferBuilder& builder);
			std::shared_ptr<Framebuffer> get(std::string name);
			void clean();

		private:
			Context& _context;
			std::unordered_map<std::string, std::shared_ptr<Framebuffer>> _framebuffers;
	};
}

#endif