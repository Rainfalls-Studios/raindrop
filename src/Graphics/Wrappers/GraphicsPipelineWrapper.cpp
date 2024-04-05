#include <Raindrop/Graphics/Wrappers/GraphicsPipelineWrapper.hpp>
#include <Raindrop/Graphics/Pipelines/GraphicsPipeline.hpp>
#include <Raindrop/Graphics/Context.hpp>

namespace Raindrop::Graphics::Wrappers{
	GraphicsPipelineWrapper::GraphicsPipelineWrapper(Context& context) : 
		_context{&context},
		_ID{Pipelines::INVALID_LAYOUT_ID}{}
	
	GraphicsPipelineWrapper::GraphicsPipelineWrapper(Context& context, const GraphicsPipelineID& ID) : 
		_context{&context},
		_ID{ID}{}
	
	GraphicsPipelineWrapper::GraphicsPipelineWrapper(const GraphicsPipelineWrapper& other) : 
		_context{other._context},
		_ID{other._ID}{}

	GraphicsPipelineWrapper& GraphicsPipelineWrapper::operator=(const GraphicsPipelineWrapper& other){
		_context = other._context;
		_ID = other._ID;
		return *this;
	}

	GraphicsPipelineWrapper& GraphicsPipelineWrapper::operator=(const GraphicsPipelineID& ID){
		_ID = ID;
		return *this;
	}

	GraphicsPipelineWrapper::~GraphicsPipelineWrapper(){}

	Pipelines::GraphicsPipeline& GraphicsPipelineWrapper::get(){
		assert(_context != nullptr && "The pipeline GraphicsPipeline wrapper context is invalid");
		// TODO : secutiry check, the handle might be nullptr
		return *_context->graphicsPipelineRegistry.get(_ID);
	}

	const Pipelines::GraphicsPipeline& GraphicsPipelineWrapper::get() const{
		assert(_context != nullptr && "The pipeline GraphicsPipeline wrapper context is invalid");
		// TODO : secutiry check, the handle might be nullptr
		return *_context->graphicsPipelineRegistry.get(_ID);
	}

	GraphicsPipelineWrapper::GraphicsPipelineID GraphicsPipelineWrapper::ID() const{
		return _ID;
	}

	GraphicsPipelineWrapper::GraphicsPipeline* GraphicsPipelineWrapper::operator->(){
		return &get();
	}

	const GraphicsPipelineWrapper::GraphicsPipeline* GraphicsPipelineWrapper::operator->() const{
		return &get();
	}
}