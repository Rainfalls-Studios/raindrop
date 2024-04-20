#include <Raindrop/Graphics/RenderSystems/RenderSystemCollection.hpp>
#include <Raindrop/Graphics/RenderSystems/RenderSystem.hpp>

#include <spdlog/spdlog.h>

namespace Raindrop::Graphics::RenderSystems{
	RenderSystemCollection::RenderSystemCollection(Context& context) : _context{context}{}
	RenderSystemCollection::~RenderSystemCollection(){}

	void RenderSystemCollection::pushRenderSystem(const std::shared_ptr<RenderSystem>& system){
		_renderSystems.push_back(system);
	}

	void RenderSystemCollection::pushRenderSystems(const std::initializer_list<std::shared_ptr<RenderSystem>>& systems){
		for (const auto& system : systems){
			pushRenderSystem(system);
		}
	}

	void RenderSystemCollection::render(VkCommandBuffer commandBuffer, ::Raindrop::Wrappers::SceneWrapper scene){
		for (auto& system : _renderSystems){
			try{
				system->render(commandBuffer, scene);
			} catch (const std::exception& e){
				spdlog::error("Render error : {}", e.what());
			}
		}
	}
}