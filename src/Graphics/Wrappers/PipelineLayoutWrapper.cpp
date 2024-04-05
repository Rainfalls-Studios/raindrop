#include <Raindrop/Graphics/Wrappers/PipelineLayoutWrapper.hpp>
#include <Raindrop/Graphics/Pipelines/PipelineLayout.hpp>
#include <Raindrop/Graphics/Context.hpp>

namespace Raindrop::Graphics::Wrappers{

	Pipelines::PipelineLayout& PipelineLayoutWrapper::get(){
		assert(_context != nullptr && "The pipeline layout wrapper context is invalid");
		auto ptr = _context->pipelineLayoutRegistry.get(_ID);
		if (ptr == nullptr) throw std::runtime_error("Invalid pipeline layout ID");
		return *ptr;
	}

	const Pipelines::PipelineLayout& PipelineLayoutWrapper::get() const{
		assert(_context != nullptr && "The pipeline layout wrapper context is invalid");
		auto ptr = _context->pipelineLayoutRegistry.get(_ID);
		if (ptr == nullptr) throw std::runtime_error("Invalid pipeline layout ID");
		return *ptr;
	}
	
	VkPipelineLayout PipelineLayoutWrapper::layout() const{
		return get().get();
	}
}