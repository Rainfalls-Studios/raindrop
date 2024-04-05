#include <Raindrop/Graphics/Pipelines/GraphicsPipelineRegistry.hpp>
#include <Raindrop/Graphics/Pipelines/GraphicsPipeline.hpp>
#include <Raindrop/Graphics/Context.hpp>

#include <spdlog/spdlog.h>

namespace Raindrop::Graphics::Pipelines{
	GraphicsPipelineRegistry::GraphicsPipelineRegistry(Context& context) : _context{context}{
		spdlog::info("Creating graphics pipeline registry...");
	}

	GraphicsPipelineRegistry::~GraphicsPipelineRegistry(){
		spdlog::info("Destroying graphics pipeline registry ... ({} pipelines)", _pipelines.size());
		_pipelines.clear();
	}

	GraphicsPipelineID GraphicsPipelineRegistry::create(const GraphicsPipelineConfigInfo& info){
		std::unique_ptr<GraphicsPipeline> pipeline = std::make_unique<GraphicsPipeline>(_context, info);

		if (_freeIDs.empty()){
			_pipelines.push_back(std::move(pipeline));
			return static_cast<LayoutID>(_pipelines.size() - 1);
		}

		LayoutID ID = _freeIDs.front();
		_freeIDs.pop();

		_pipelines[ID] = std::move(pipeline);
		return ID;
	}

	void GraphicsPipelineRegistry::destroy(const GraphicsPipelineID& ID){
		assert(ID < _pipelines.size() && "Invalid graphics pipeline ID");
		_pipelines[ID].reset();
		_freeIDs.push(ID);
	}

	GraphicsPipeline* GraphicsPipelineRegistry::get(const GraphicsPipelineID& ID){
		assert(ID < _pipelines.size() && "Invalid graphics pipeline ID");
		return _pipelines[ID].get();
	}
}