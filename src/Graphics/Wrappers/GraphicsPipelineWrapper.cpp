#include <Raindrop/Graphics/Wrappers/GraphicsPipelineWrapper.hpp>
#include <Raindrop/Graphics/Wrappers/PipelineLayoutWrapper.hpp>
#include <Raindrop/Graphics/Pipelines/GraphicsPipeline.hpp>
#include <Raindrop/Graphics/Context.hpp>

namespace Raindrop::Graphics::Wrappers{
	Pipelines::GraphicsPipeline& GraphicsPipelineWrapper::get(){
		assert(_context != nullptr && "The pipeline GraphicsPipeline wrapper context is invalid");
		auto ptr = Wrapper::_context->graphicsPipelineRegistry.get(_ID);
		if (ptr == nullptr) throw std::runtime_error("Invalid graphics pipeline ID");
		return *ptr;
	}

	const Pipelines::GraphicsPipeline& GraphicsPipelineWrapper::get() const{
		assert(_context != nullptr && "The pipeline GraphicsPipeline wrapper context is invalid");
		auto ptr = Wrapper::_context->graphicsPipelineRegistry.get(_ID);
		if (ptr == nullptr) throw std::runtime_error("Invalid graphics pipeline ID");
		return *ptr;
	}

	VkPipeline GraphicsPipelineWrapper::pipeline() const{
		return get().get();
	}

	void GraphicsPipelineWrapper::bind(VkCommandBuffer commandBuffer){
		get().bind(commandBuffer);
	}

	Pipelines::LayoutID GraphicsPipelineWrapper::layoutID() const{
		return Pipelines::INVALID_LAYOUT_ID;
	}

	PipelineLayoutWrapper GraphicsPipelineWrapper::layout() const{
		return PipelineLayoutWrapper(*_context, layoutID());
	}
}