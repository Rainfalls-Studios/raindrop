#ifndef __RAINDROP_RENDERER_MATERIAL_CONTEXT_HPP__
#define __RAINDROP_RENDERER_MATERIAL_CONTEXT_HPP__

#include "common.hpp"
#include "Pool.hpp"
#include "Buffer.hpp"
#include "SetLayout.hpp"
#include "Registry.hpp"
#include "../Context.hpp"


// Should be enought
// TODO: make this cleaner with a dynamic descriptor pool creation system
#define POOL_SIZE 1000

namespace Raindrop::Renderer::Material{
	struct Context{
		using Renderer = ::Raindrop::Renderer::Context;
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