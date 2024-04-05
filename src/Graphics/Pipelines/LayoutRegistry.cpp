#include <Raindrop/Graphics/Pipelines/LayoutRegistry.hpp>
#include <Raindrop/Graphics/Pipelines/PipelineLayout.hpp>
#include <Raindrop/Graphics/Context.hpp>

#include <spdlog/spdlog.h>

namespace Raindrop::Graphics::Pipelines{
	LayoutRegistry::LayoutRegistry(Context& context) : _context{context}{
		spdlog::info("Creating pipeline layout registry ...");
	}

	LayoutRegistry::~LayoutRegistry(){
		spdlog::info("Destroying pipeline layout registry ... ({} layout(s))", Registry::usedSize());
	}

	LayoutID LayoutRegistry::create(const PipelineLayoutConfigInfo& info){
		return Registry::registerHandle(std::make_unique<PipelineLayout>(_context, info));
	}

	void LayoutRegistry::destroy(const LayoutID& ID){
		Registry::destroy(ID);
	}

	PipelineLayout* LayoutRegistry::get(const LayoutID& ID){
		return Registry::get(ID).get();
	}
}