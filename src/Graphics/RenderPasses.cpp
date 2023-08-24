#include <Raindrop/Graphics/RenderPasses.hpp>

namespace Raindrop::Graphics{
	RenderPasses::RenderPasses(GraphicsContext& context) : 
		sun{context},
		forwardShader{context},
		scene{context}
	{}

	RenderPasses::~RenderPasses(){}
}