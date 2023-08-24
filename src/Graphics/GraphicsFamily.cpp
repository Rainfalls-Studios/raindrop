#include <Raindrop/Graphics/GraphicsFamily.hpp>

namespace Raindrop::Graphics{
	GraphicsFamily::GraphicsFamily(GraphicsContext& context) : 
		commandPool{context},
		commandBuffers{context},
		_context{context}
	{}
}