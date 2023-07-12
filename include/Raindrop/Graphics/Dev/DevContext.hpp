#ifndef __RAINDROP_GRAPHICS_DEV_CONTEXT_HPP__
#define __RAINDROP_GRAPHICS_DEV_CONTEXT_HPP__

#include <Raindrop/Graphics/Dev/common.hpp>
#include <Raindrop/Graphics/GraphicsContext.hpp>

namespace Raindrop::Graphics::Dev{
	struct DevContext{
		GraphicsContext& context;
	};
}

#endif