#ifndef __RAINDROP_COMPONENTS_MODEL_HPP__
#define __RAINDROP_COMPONENTS_MODEL_HPP__

#include "common.hpp"
#include <Raindrop/Renderer/Model/Model.hpp>

namespace Raindrop::Components{
	struct Model{
		Model();

		std::weak_ptr<Renderer::Model::Model> model;

		
	};
}

#endif