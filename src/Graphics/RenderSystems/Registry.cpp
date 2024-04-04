#include <Raindrop/Graphics/RenderSystems/Registry.hpp>
#include <Raindrop/Graphics/RenderSystems/RenderSystem.hpp>
#include <Raindrop/Graphics/Context.hpp>

#include <spdlog/spdlog.h>

namespace Raindrop::Graphics::RenderSystems{
	Registry::Registry(Context& context) : _context{context}{
		spdlog::info("Constructing Renderer render system registry ...");
	}

	Registry::~Registry(){
		spdlog::info("Destroying Renderer render system registry ...");
		_renderSystems.clear();
	}

	
	RenderSystemID Registry::registerRenderSystem(std::unique_ptr<RenderSystem> system){
		spdlog::info("Registring render system ...");
		if (_freeIDs.empty()){
			_renderSystems.push_back(std::move(system));
			return  static_cast<RenderSystemID>(_renderSystems.size() - 1);
		}

		RenderSystemID ID  = _freeIDs.front();
		_freeIDs.pop();

		_renderSystems[ID] = std::move(system);
		return ID;
	}

	void Registry::destroyRenderSystem(const RenderSystemID& ID){
		assert(static_cast<std::size_t>(ID < _renderSystems.size()) && "The render system ID is out of range");
		assert(_renderSystems[ID] != nullptr && "Invalid system ID");

		_renderSystems[ID].reset();
		_freeIDs.push(ID);
	}
			
	RenderSystem& Registry::getRenderSystem(const RenderSystemID& ID){
		assert(static_cast<std::size_t>(ID < _renderSystems.size()) && "The render system ID is out of range");
		assert(_renderSystems[ID] != nullptr && "Invalid system ID");

		return *_renderSystems[ID];
	}
}