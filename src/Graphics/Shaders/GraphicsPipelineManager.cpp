#include <Raindrop/Graphics/Shaders/GraphicsPipelineManager.hpp>
#include <Raindrop/Graphics/Shaders/GraphicsPipeline.hpp>
#include <Raindrop/Graphics/Shaders/GraphicsPipelineBuilder.hpp>
#include <Raindrop/Graphics/Shaders/Context.hpp>

namespace Raindrop::Graphics::Shaders{
	GraphicsPipelineManager::GraphicsPipelineManager(Context& context) : _context{context}{}
	GraphicsPipelineManager::~GraphicsPipelineManager(){
		_pipelines.clear();
	}

	std::shared_ptr<GraphicsPipeline> GraphicsPipelineManager::create(const GraphicsPipelineBuilder& builder){
		std::shared_ptr<GraphicsPipeline> pipeline = std::make_shared<GraphicsPipeline>(_context, builder);
		_pipelines[builder.name()] = pipeline;
		return pipeline;
	}

	std::shared_ptr<GraphicsPipeline> GraphicsPipelineManager::get(const std::string& name){
		auto it = _pipelines.find(name);
		if (it == _pipelines.end()){
			_context.logger().warn("Cannot find \"{}\" graphics pipeline", name);
			return nullptr;
		}
		return it->second;
	}

	void GraphicsPipelineManager::clean(){
		std::erase_if(
			_pipelines,
			[](const auto& iterator){return iterator.second.unique();}
		);
	}
}