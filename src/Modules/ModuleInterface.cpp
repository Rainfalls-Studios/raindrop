#include <Raindrop/Modules/ModuleInterface.hpp>
#include <Raindrop/Graphics/RenderSystems/RenderSystem.hpp>
#include <Raindrop/Graphics/RenderSystems/RenderSystemCollection.hpp>

namespace Raindrop::Modules{
	const std::unordered_map<std::string, RenderSystemCollection>& ModuleInterface::renderSystemCollections() const{
		static std::unordered_map<std::string, RenderSystemCollection> empty;
		return empty;
	}
}