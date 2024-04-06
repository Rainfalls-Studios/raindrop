#ifndef __RAINDROP_TESTBED_FORWARD_RENDER_SYSTEM_HPP__
#define __RAINDROP_TESTBED_FORWARD_RENDER_SYSTEM_HPP__

#include <Raindrop/Raindrop.hpp>
#include "config.h"

struct ForwardRenderSceneProperties : public Raindrop::SceneProperty{
	glm::vec4 ambientColor;
};

class ForwardRenderSystem : public Raindrop::Graphics::RenderSystem{
	public:
		ForwardRenderSystem(Raindrop::Raindrop& engine);
		~ForwardRenderSystem();

		void render(Raindrop::SceneWrapper& scene);

	private:
		Raindrop::Raindrop& _engine;
};

#endif