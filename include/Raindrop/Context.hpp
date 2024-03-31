#ifndef __RAINDROP_CONTEXT_HPP__
#define __RAINDROP_CONTEXT_HPP__

#include "common.hpp"
#include "Core/Asset/Manager.hpp"
#include "Core/Event/Manager.hpp"
#include "Graphics/Renderer.hpp"
#include "Core/Scene/Scenes.hpp"
#include "Camera.hpp"

namespace Raindrop{
	struct Context{
		Context();
		~Context() = default;

		Core::Asset::Manager assetManager;
		Core::Event::Manager eventManager;
		Core::Scene::Scenes scenes;
		Camera camera;
		Graphics::Renderer renderer;
		

		bool running;
	};
}

#endif