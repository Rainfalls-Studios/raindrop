#include <Raindrop/Graphics/RenderSystems/RenderSystemRegistry.hpp>
#include <Raindrop/Graphics/RenderSystems/RenderSystem.hpp>
#include <Raindrop/Graphics/Context.hpp>

#include <spdlog/spdlog.h>

namespace Raindrop::Graphics::RenderSystems{
	RenderSystemRegistry::RenderSystemRegistry(Context& context) : _context{context}{
		spdlog::info("Creating graphics pipeline registry ...");
	}

	RenderSystemRegistry::~RenderSystemRegistry(){
		spdlog::info("Destroying graphics pipeline registry ... ({} pipeline(s))", Registry::usedSize());
	}

	RenderSystemID RenderSystemRegistry::create(std::unique_ptr<RenderSystem>&& system){
		return Registry::registerHandle(std::move(system));
	}

	void RenderSystemRegistry::destroy(const RenderSystemID& ID){
		Registry::destroy(ID);
	}

	RenderSystem* RenderSystemRegistry::get(const RenderSystemID& ID){
		return Registry::get(ID).get();
	}
}