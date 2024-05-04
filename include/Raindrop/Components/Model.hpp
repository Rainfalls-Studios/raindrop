#ifndef __RAINDROP_COMPONENTS_MODELS_HPP__
#define __RAINDROP_COMPONENTS_MODELS_HPP__

#include "common.hpp"
#include <Raindrop/Graphics/Models/Model.hpp>

namespace Raindrop::Components{
	class Model{
		public:
			Model();

			inline void setModel(const std::weak_ptr<Graphics::Models::Model>& model);
			inline std::weak_ptr<Graphics::Models::Model>& getModel();
			inline const std::weak_ptr<Graphics::Models::Model>& getModel() const;

		private:
			std::weak_ptr<Graphics::Models::Model> _model;
	};
}

#include "Model.inl"

#endif