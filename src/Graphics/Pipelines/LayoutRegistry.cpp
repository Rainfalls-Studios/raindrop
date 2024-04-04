#include <Raindrop/Graphics/Pipelines/LayoutRegistry.hpp>
#include <Raindrop/Graphics/Pipelines/PipelineLayout.hpp>
#include <Raindrop/Graphics/Context.hpp>

#include <spdlog/spdlog.h>

namespace Raindrop::Graphics::Pipelines{
	LayoutRegistry::LayoutRegistry(Context& context) : _context{context}{
		spdlog::info("Creating pipeline layout registry...");
	}

	LayoutRegistry::~LayoutRegistry(){
		spdlog::info("Destroying pipeline layout registry ... ({} layouts)", _layouts.size());
		_layouts.clear();
	}

	LayoutID LayoutRegistry::create(const PipelineLayoutConfigInfo& info){
		std::unique_ptr<PipelineLayout> layout = std::make_unique<PipelineLayout>(_context, info);

		if (_freeIDs.empty()){
			_layouts.push_back(std::move(layout));
			return static_cast<LayoutID>(_layouts.size() - 1);
		}

		LayoutID ID = _freeIDs.front();
		_freeIDs.pop();

		_layouts[ID] = std::move(layout);
		return ID;
	}

	void LayoutRegistry::destroy(const LayoutID& ID){
		assert(ID < _layouts.size() && "Invalid layout ID");
		_layouts[ID].reset();
		_freeIDs.push(ID);
	}

	PipelineLayout* LayoutRegistry::get(const LayoutID& ID){
		assert(ID < _layouts.size() && "Invalid layout ID");
		return _layouts[ID].get();
	}
}