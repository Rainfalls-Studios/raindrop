#ifndef __RAINDROP_INTERNAL_RENDER_PASSES_REGISTRY_HPP__
#define __RAINDROP_INTERNAL_RENDER_PASSES_REGISTRY_HPP__

#include "common.hpp"
#include <Raindrop_internal/Templates/Registry.hpp>

namespace Raindrop::Internal::RenderPasses{
	using Registry = Templates::Registry<RenderPass>;
	using Handle = Registry::Handle;
	static constexpr Handle INVALID_HANDLE = Registry::INVALID_HANDLE;
}

#endif