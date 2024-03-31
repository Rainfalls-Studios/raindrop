#ifndef __RAINDROP_GRAPHICS_RENDER_SYSTEMS_COMMON_HPP__
#define __RAINDROP_GRAPHICS_RENDER_SYSTEMS_COMMON_HPP__

#include "../common.hpp"
#include <Raindrop/Core/Scene/common.hpp>

namespace Raindrop::Graphics::RenderSystems{
	using Scene = ::Raindrop::Core::Scene::Scene;
	using SceneID = ::Raindrop::Core::Scene::SceneID;

	using RenderSystemID = std::uint32_t;

	class RenderSystem;
	class Registry;
}

#endif