#include <Raindrop/Modules/ModuleInterface.hpp>
#include <Raindrop/Graphics/RenderSystems/RenderSystem.hpp>
#include <Raindrop/Graphics/RenderSystems/RenderSystemCollection.hpp>

namespace Raindrop::Modules{
	const std::vector<RenderSystemID>& ModuleInterface::renderSystemIDs() const{
		static const std::vector<RenderSystemID> empty;
		return empty;
	}

	const std::vector<RenderSystemCollection>& ModuleInterface::renderSystemCollections() const{
		static const std::vector<RenderSystemCollection> empty;
		return empty;
	}

}