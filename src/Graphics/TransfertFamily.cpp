#include <Raindrop/Graphics/TransfertFamily.hpp>

namespace Raindrop::Graphics{
	TransfertFamily::TransfertFamily(GraphicsContext& context) : 
		commandPool{context},
		_context{context}{}
}