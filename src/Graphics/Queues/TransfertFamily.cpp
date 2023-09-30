#include <Raindrop/Graphics/Queues/TransfertFamily.hpp>

namespace Raindrop::Graphics::Queues{
	TransfertFamily::TransfertFamily(GraphicsContext& context) : 
		commandPool{context},
		_context{context}{}
}