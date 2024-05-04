#include <Raindrop/Graphics/RenderSystems/RenderSystem.hpp>

namespace Raindrop::Graphics::RenderSystems{
	std::string RenderSystem::getName() const{
		return "None";
	}

	std::string RenderSystem::getDescription() const{
		return "None";
	}

	Version RenderSystem::getVersion() const{
		return Version(0, 1, 0, 0);
	}
}