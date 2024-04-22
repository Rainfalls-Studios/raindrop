#include <Raindrop/Graphics/RenderSystems/RenderSystem.hpp>

namespace Raindrop::Graphics::RenderSystems{
	std::string RenderSystem::name() const{
		return "None";
	}

	std::string RenderSystem::description() const{
		return "None";
	}

	Version RenderSystem::version() const{
		return Version(0, 1, 0, 0);
	}
}