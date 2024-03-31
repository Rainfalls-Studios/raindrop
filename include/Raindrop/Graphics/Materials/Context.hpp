#ifndef __RAINDROP_GRAPHICS_MATERIAL_CONTEXT_HPP__
#define __RAINDROP_GRAPHICS_MATERIAL_CONTEXT_HPP__

#include "common.hpp"
#include "Pool.hpp"
#include "Buffer.hpp"
#include "SetLayout.hpp"
#include "Registry.hpp"
#include "../Context.hpp"


// Should be enought
// TODO: make this cleaner with a dynamic descriptor pool creation system
#define POOL_SIZE 1000

namespace Raindrop::Graphics::Materials{
	struct Context{
		using Renderer = ::Raindrop::Graphics::Context;
		Context(Renderer& renderer);

		Renderer& renderer;
		std::size_t instanceCount;
		
		SetLayout layout;
		Pool pool;
		Buffer buffer;
		Registry registry;
	};
}

#endif