#ifndef __RAINDROP_GRAPHICS_BUFFERS_CONTEXT_HPP__
#define __RAINDROP_GRAPHICS_BUFFERS_CONTEXT_HPP__

#include <spdlog/spdlog.h>
#include <Raindrop/Graphics/Buffers/common.hpp>
#include <Raindrop/Graphics/Context.hpp>
#include <Raindrop/Graphics/Buffers/common.hpp>
#include <Raindrop/Graphics/Buffers/VertexLayout.hpp>
#include <Raindrop/Graphics/Buffers/VertexLayoutRegistry.hpp>
#include <Raindrop/Graphics/Buffers/Loader.hpp>

namespace Raindrop::Graphics::Buffers{
	class Context{
		public:
			Context(Graphics::Context& graphics);

			Graphics::Context& graphics();
			spdlog::logger& logger();
			VertexLayoutRegistry& vertexdLayoutRegistry();
			Loader& loader();
		
		private:
			Graphics::Context& _graphics;
			std::shared_ptr<spdlog::logger> _logger;

			std::unique_ptr<VertexLayoutRegistry> _layoutRegistry;
			std::unique_ptr<Loader> _loader;

	};
}

#endif