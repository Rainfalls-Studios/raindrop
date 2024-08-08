#ifndef __RAINDROP_GRAPHICS_SCENE_COMPONENTS_MODEL_HPP__
#define __RAINDROP_GRAPHICS_SCENE_COMPONENTS_MODEL_HPP__

#include "../types.hpp"
#include "../pch.pch"
#include <Raindrop/Graphics/types.hpp>

namespace Raindrop::Graphics::Scene::Components{
	class Model{
		public:
			std::shared_ptr<Graphics::Model> model;

		private:
			// std::list<Entity> children;
	};
}

#endif