#ifndef __RAINDROP_GRAPHICS_BUFFERS_CONTEXT_HPP__
#define __RAINDROP_GRAPHICS_BUFFERS_CONTEXT_HPP__

#include <spdlog/spdlog.h>
#include <Raindrop/Graphics/Context.hpp>
#include <Raindrop/Graphics/Buffers/common.hpp>
#include <Raindrop/Graphics/Buffers/VertexLayout.hpp>

namespace Raindrop::Graphics::Buffers{
	class Context{
		public:
			Context(Graphics::Context& graphics);

			Graphics::Context& graphics();
			spdlog::logger& logger();
			VertexLayout& defaultLayout();
		
		private:
			Graphics::Context& _graphics;
			std::shared_ptr<spdlog::logger> _logger;
			std::unique_ptr<VertexLayout> _defaultLayout;

	};
}

#endif