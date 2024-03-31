#ifndef __RAINDROP_COMPONENTS_MODELS_HPP__
#define __RAINDROP_COMPONENTS_MODELS_HPP__

#include "common.hpp"
#include <Raindrop/Graphics/Models/Model.hpp>

namespace Raindrop::Components{
	struct Model{
		Model();

		std::weak_ptr<Graphics::Models::Model> model;
	};
}

#endif