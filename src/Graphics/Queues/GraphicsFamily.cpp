#include <Raindrop/Graphics/Queues/GraphicsFamily.hpp>

namespace Raindrop::Graphics::Queues{
	GraphicsFamily::GraphicsFamily(GraphicsContext& context) : 
		commandPool{context},
		commandBuffers{context},
		_context{context}
	{}
}