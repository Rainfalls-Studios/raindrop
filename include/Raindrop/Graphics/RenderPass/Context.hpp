#ifndef __RAINDROP_GRAPHICS_RENDER_PASS_CONTEXT_HPP__
#define __RAINDROP_GRAPHICS_RENDER_PASS_CONTEXT_HPP__

#include <Raindrop/Graphics/RenderPass/common.hpp>
#include <Raindrop/Graphics/Context.hpp>
#include <Raindrop/Graphics/RenderPass/RenderPassManager.hpp>
#include <Raindrop/Graphics/RenderPass/RenderPass.hpp>
#include <Raindrop/Graphics/RenderPass/Loader.hpp>
#include <spdlog/logger.h>

namespace Raindrop::Graphics::RenderPass{
	class Context{
		public:
			Context(Graphics::Context& graphics);
			~Context();

			Graphics::Context& graphics();
			spdlog::logger& logger();
			RenderPassManager& renderPassManager();
			Loader& loader();

		private:
			Graphics::Context& _graphics;
			std::shared_ptr<spdlog::logger> _logger;

			std::unique_ptr<RenderPassManager> _renderPassManager;
			std::unique_ptr<Loader> _loader;
	};
}

#endif