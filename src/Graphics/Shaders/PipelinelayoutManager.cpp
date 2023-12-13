#include <Raindrop/Graphics/Shaders/PipelineLayoutManager.hpp>
#include <Raindrop/Graphics/Shaders/PipelineLayout.hpp>
#include <Raindrop/Graphics/Shaders/PipelineLayoutBuilder.hpp>
#include <Raindrop/Graphics/Shaders/Context.hpp>

namespace Raindrop::Graphics::Shaders{
	PipelineLayoutManager::PipelineLayoutManager(Context& context) : _context{context}{}
	PipelineLayoutManager::~PipelineLayoutManager(){}

	std::shared_ptr<PipelineLayout> PipelineLayoutManager::create(const PipelineLayoutBuilder& builder){
		std::shared_ptr<PipelineLayout> layout = std::make_shared<PipelineLayout>(_context, builder);
		_layouts[builder.name()] = layout;
		return layout;
	}

	std::shared_ptr<PipelineLayout> PipelineLayoutManager::get(const std::string& name) const{
		auto it = _layouts.find(name);
		if (it == _layouts.end()){
			_context.logger().warn("Cannot find \"{}\" pipeline layout", name);
			return nullptr;
		}
		return it->second;
	}
			
	void PipelineLayoutManager::clean(){
		std::erase_if(
			_layouts,
			[](const auto& iterator){return iterator.second.unique();}
		);
	}
}