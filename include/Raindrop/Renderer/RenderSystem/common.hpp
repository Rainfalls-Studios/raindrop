#ifndef __RAINDROP_RENDERER_RENDER_SYSTEM_HPP__
#define __RAINDROP_RENDERER_RENDER_SYSTEM_HPP__

#include "../common.hpp"
#include <Raindrop/Core/Scene/common.hpp>

namespace Raindrop::Renderer::RenderSystem{
	using Scene = ::Raindrop::Core::Scene::Scene;
	using SceneID = ::Raindrop::Core::Scene::SceneID;

	using RenderSystemID = std::uint32_t;

	class RenderSystem;
	class Registry;
}

#endif