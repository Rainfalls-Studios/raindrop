#ifndef __RAINDROP_GRAPHICS_BUFFERS_CONTEXT_HPP__
#define __RAINDROP_GRAPHICS_BUFFERS_CONTEXT_HPP__

#include <spdlog/spdlog.h>
#include <Raindrop/Graphics/Context.hpp>
#include <Raindrop/Graphics/Buffers/common.hpp>
#include <Raindrop/Graphics/Buffers/VertexLayout.hpp>

namespace Raindrop::Graphics::Buffers{
	struct Context{
		Graphics::Context& graphics;
		spdlog::logger logger;

		VertexLayout defaultLayout;

		Context(Graphics::Context& graphics);
	};
}

#endif