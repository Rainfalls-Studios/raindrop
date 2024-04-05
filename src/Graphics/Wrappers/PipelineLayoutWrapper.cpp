#include <Raindrop/Graphics/Wrappers/PipelineLayoutWrapper.hpp>
#include <Raindrop/Graphics/Pipelines/PipelineLayout.hpp>
#include <Raindrop/Graphics/Context.hpp>

namespace Raindrop::Graphics::Wrappers{
	PipelineLayoutWrapper::PipelineLayoutWrapper(Context& context) : 
		_context{&context},
		_ID{Pipelines::INVALID_LAYOUT_ID}{}
	
	PipelineLayoutWrapper::PipelineLayoutWrapper(Context& context, const LayoutID& ID) : 
		_context{&context},
		_ID{ID}{}
	
	PipelineLayoutWrapper::PipelineLayoutWrapper(const PipelineLayoutWrapper& other) : 
		_context{other._context},
		_ID{other._ID}{}

	PipelineLayoutWrapper& PipelineLayoutWrapper::operator=(const PipelineLayoutWrapper& other){
		_context = other._context;
		_ID = other._ID;
		return *this;
	}

	PipelineLayoutWrapper& PipelineLayoutWrapper::operator=(const LayoutID& ID){
		_ID = ID;
		return *this;
	}

	PipelineLayoutWrapper::~PipelineLayoutWrapper(){}

	Pipelines::PipelineLayout& PipelineLayoutWrapper::get(){
		assert(_context != nullptr && "The pipeline layout wrapper context is invalid");
		// TODO : secutiry check, the handle might be nullptr
		return *_context->pipelineLayoutRegistry.get(_ID);
	}

	const Pipelines::PipelineLayout& PipelineLayoutWrapper::get() const{
		assert(_context != nullptr && "The pipeline layout wrapper context is invalid");
		// TODO : secutiry check, the handle might be nullptr
		return *_context->pipelineLayoutRegistry.get(_ID);
	}

	PipelineLayoutWrapper::LayoutID PipelineLayoutWrapper::ID() const{
		return _ID;
	}

	PipelineLayoutWrapper::Layout* PipelineLayoutWrapper::operator->(){
		return &get();
	}

	const PipelineLayoutWrapper::Layout* PipelineLayoutWrapper::operator->() const{
		return &get();
	}

	VkPipelineLayout PipelineLayoutWrapper::layout() const{
		return get().get();
	}
}