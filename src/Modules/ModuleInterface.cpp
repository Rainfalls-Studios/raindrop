#include <Raindrop/Modules/ModuleInterface.hpp>

namespace Raindrop::Modules{
	const std::vector<std::shared_ptr<RenderSystem>>& ModuleInterface::renderSystems() const{
		static const std::vector<std::shared_ptr<RenderSystem>> empty;
		return empty;
	}
}