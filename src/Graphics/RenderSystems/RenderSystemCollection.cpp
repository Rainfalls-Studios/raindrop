#include <Raindrop/Graphics/RenderSystems/RenderSystemCollection.hpp>
#include <Raindrop/Graphics/RenderSystems/RenderSystem.hpp>

#include <spdlog/spdlog.h>

namespace Raindrop::Graphics::RenderSystems{
	RenderSystemCollection::RenderSystemCollection(const std::string& name, const std::initializer_list<std::pair<std::shared_ptr<RenderSystem>, std::string>>& systems, const std::string& description) : 
			_name{name},
			_description{description}
	{
		for (const auto& pair : systems){
			const auto& system = pair.first;
			const auto& name = pair.second;
			
			_renderSystems.insert_or_assign(name, system);
		}
	}

	const std::string& RenderSystemCollection::name() const{
		return _name;
	}

	const std::string& RenderSystemCollection::descrition() const{
		return _description;
	}

	std::shared_ptr<RenderSystem> RenderSystemCollection::get(const std::string& name) const{
		auto it = _renderSystems.find(name);
		if (it == _renderSystems.end()){
			spdlog::warn("Cannot locate render system name \"{}\" in collection", name);
			return nullptr;
		}
		return it->second;
	}

	std::vector<std::shared_ptr<RenderSystem>> RenderSystemCollection::systems() const{
		std::vector<std::shared_ptr<RenderSystem>> systems(_renderSystems.size());

		std::size_t i=0;
		for (const auto& pair : _renderSystems){
			systems[i] = pair.second;
			i++;
		}

		return systems;
	}
}