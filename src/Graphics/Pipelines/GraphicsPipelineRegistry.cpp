#include <Raindrop/Graphics/Pipelines/GraphicsPipelineRegistry.hpp>
#include <Raindrop/Graphics/Pipelines/GraphicsPipeline.hpp>
#include <Raindrop/Graphics/Context.hpp>

#include <spdlog/spdlog.h>

namespace Raindrop::Graphics::Pipelines{
	GraphicsPipelineRegistry::GraphicsPipelineRegistry(Context& context) : _context{context}{
		spdlog::info("Creating graphics pipeline registry ...");
	}

	GraphicsPipelineRegistry::~GraphicsPipelineRegistry(){
		spdlog::info("Destroying graphics pipeline registry ... ({} pipeline(s))", Registry::usedSize());
	}

	GraphicsPipelineID GraphicsPipelineRegistry::create(const GraphicsPipelineConfigInfo& info){
		return Registry::registerHandle(std::make_unique<GraphicsPipeline>(_context, info));
	}

	void GraphicsPipelineRegistry::destroy(const GraphicsPipelineID& ID){
		Registry::destroy(ID);
	}

	GraphicsPipeline* GraphicsPipelineRegistry::get(const GraphicsPipelineID& ID){
		return Registry::get(ID).get();
	}
}