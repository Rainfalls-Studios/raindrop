#include <Raindrop/Graphics/Pipelines.hpp>

namespace Raindrop::Graphics{
	Pipelines::Pipelines(GraphicsContext& context) :
		_context{context},
		sun{context}{
	}

	Pipelines::~Pipelines(){}
}