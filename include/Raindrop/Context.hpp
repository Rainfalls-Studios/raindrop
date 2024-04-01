#ifndef __RAINDROP_CONTEXT_HPP__
#define __RAINDROP_CONTEXT_HPP__

#include "common.hpp"
#include "Core/Assets/Manager.hpp"
#include "Core/Event/Manager.hpp"
#include "Graphics/Renderer.hpp"
#include "Core/Scenes/Registry.hpp"
#include "Camera.hpp"

namespace Raindrop{
	struct Context{
		Context();
		~Context() = default;

		Core::Assets::Manager assetManager;
		Core::Event::Manager eventManager;
		Core::Scenes::Registry scenes;
		Camera camera;
		Graphics::Renderer renderer;
		

		bool running;
	};
}

#endif