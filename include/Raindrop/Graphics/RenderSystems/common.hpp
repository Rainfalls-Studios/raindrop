#ifndef __RAINDROP_GRAPHICS_RENDER_SYSTEMS_COMMON_HPP__
#define __RAINDROP_GRAPHICS_RENDER_SYSTEMS_COMMON_HPP__

#include "../common.hpp"
#include <Raindrop/Core/Scenes/common.hpp>
#include <Raindrop/Wrappers/SceneWrapper.hpp>

namespace Raindrop::Graphics::RenderSystems{
	using Scene = ::Raindrop::Core::Scenes::Scene;
	using SceneID = ::Raindrop::Core::Scenes::SceneID;

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