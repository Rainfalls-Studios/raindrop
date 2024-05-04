#include <Raindrop/Graphics/RenderSystems/RenderSystemCollection.hpp>
#include <Raindrop/Graphics/RenderSystems/RenderSystem.hpp>

#include <spdlog/spdlog.h>

namespace Raindrop::Graphics::RenderSystems{
	RenderSystemCollection::RenderSystemCollection(const std::initializer_list<std::pair<std::shared_ptr<RenderSystem>, std::string>>& systems, const std::initializer_list<std::pair<RenderSequence, std::string>>& sequences){
		for (const auto& pair : systems){
			const auto& system = pair.first;
			const auto& name = pair.second;
			
			_renderSystems.insert_or_assign(name, system);
		}

		for (const auto& pair : sequences){
			const auto& sequence = pair.first;
			const auto& name = pair.second;
			
			_renderSequences.insert_or_assign(name, sequence);
		}
	}

	RenderSystemCollection::RenderSystemCollection(){}

	std::shared_ptr<RenderSystem> RenderSystemCollection::getRenderSystem(const std::string& name) const{
		auto it = _renderSystems.find(name);
		if (it == _renderSystems.end()) return nullptr;
		return it->second;
	}

	std::vector<std::shared_ptr<RenderSystem>> RenderSystemCollection::renderSystems() const{
		std::vector<std::shared_ptr<RenderSystem>> systems(_renderSystems.size());

		std::size_t i=0;
		for (const auto& pair : _renderSystems){
			systems[i] = pair.second;
			i++;
		}

		return systems;
	}

	const RenderSequence* RenderSystemCollection::getRenderSequence(const std::string& name) const{
		auto it = _renderSequences.find(name);
		if (it == _renderSequences.end()) return nullptr;
		return &it->second;
	}

	std::vector<RenderSequence> RenderSystemCollection::renderSequences() const{
		std::vector<RenderSequence> senquences(_renderSequences.size());

		std::size_t i=0;
		for (const auto& pair : _renderSequences){
			senquences[i] = pair.second;
			i++;
		}

		return senquences;
	}
}