#include <Raindrop/Graphics/RenderPass/RenderPassManager.hpp>
#include <Raindrop/Graphics/RenderPass/Context.hpp>
#include <Raindrop/Graphics/RenderPass/RenderPassBuilder.hpp>
#include <Raindrop/Graphics/RenderPass/RenderPass.hpp>

namespace Raindrop::Graphics::RenderPass{
	RenderPassManager::RenderPassManager(Context& context) : _context{context}{}
	RenderPassManager::~RenderPassManager(){}

	std::shared_ptr<RenderPass> RenderPassManager::create(const RenderPassBuilder& builder){
		std::shared_ptr<RenderPass> renderPass = std::make_unique<RenderPass>(_context, builder);
		_renderPasses[builder.name()] = renderPass;
		return renderPass;
	}

	std::shared_ptr<RenderPass> RenderPassManager::get(const std::string& name){
		return _renderPasses[name];
	}

	void RenderPassManager::clean(){
		auto it = _renderPasses.begin();

		while (it != _renderPasses.end()){
			if (it->second.unique()){
				_renderPasses.erase(it);
				continue;
			}
			it++;
		}
	}
}