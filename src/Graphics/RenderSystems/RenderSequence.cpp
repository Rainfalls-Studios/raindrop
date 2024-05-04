#include <Raindrop/Graphics/RenderSystems/RenderSequence.hpp>
#include <Raindrop/Graphics/RenderSystems/RenderSystem.hpp>

namespace Raindrop::Graphics::RenderSystems{
	RenderSequence::RenderSequence(const std::initializer_list<std::shared_ptr<RenderSystem>>& systems){
		for (const auto& system : systems){
			_renderSystems.push_back(system);
		}
	}

	RenderSequence::~RenderSequence(){}

	void RenderSequence::render(VkCommandBuffer commandBuffer, ::Raindrop::Wrappers::SceneWrapper scene){
		for (auto& system : _renderSystems){
			if (system != nullptr){
				system->render(commandBuffer, scene);
			} else {
				// We might want to remove it.
			}
		}
	}

	const std::list<std::shared_ptr<RenderSystem>>& RenderSequence::renderSystems() const{
		return _renderSystems;
	}

	std::list<std::shared_ptr<RenderSystem>>& RenderSequence::renderSystems(){
		return _renderSystems;
	}
}