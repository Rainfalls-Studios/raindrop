#ifndef __RAINDROP_INTERNAL_GRAPHICS_RENDER_SYSTEMS_COMMON_HPP__
#define __RAINDROP_INTERNAL_GRAPHICS_RENDER_SYSTEMS_COMMON_HPP__

#include "../common.hpp"
#include <Raindrop/Scene.hpp>

namespace Raindrop::Internal::Graphics::RenderSystems{
	using Scene = ::Raindrop::Scene;
	using SceneID = ::Raindrop::Scene;

	using RenderSystemID = std::uint32_t;
	using RenderSystemCollectionID = std::uint32_t;

	static constexpr RenderSystemID INVALID_RENDER_SYSTEM_ID = (RenderSystemID)(~0);
	static constexpr RenderSystemCollectionID INVALID_RENDER_SYSTEM_COLLECTION_ID = (RenderSystemCollectionID)(~0);

	class RenderSystem;
	class RenderSystemRegistry;
	class RenderSystemCollection;
	class RenderSequence;
}

#endif