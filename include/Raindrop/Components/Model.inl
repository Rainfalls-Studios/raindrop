#ifndef __RAINDROP_COMPONENTS_MODEL_INL_HPP__
#define __RAINDROP_COMPONENTS_MODEL_INL_HPP__

#include "Model.hpp"

namespace Raindrop::Components{
	inline void Model::setModel(const std::weak_ptr<Graphics::Models::Model>& model){
		_model = model;
	}

	inline std::weak_ptr<Graphics::Models::Model>& Model::getModel(){
		return _model;
	}

	inline const std::weak_ptr<Graphics::Models::Model>& Model::getModel() const{
		return _model;
	}
}

#endif