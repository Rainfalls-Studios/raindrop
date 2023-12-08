#include <Raindrop/Graphics/Shaders/GraphicsPipeline.hpp>
#include <Raindrop/Graphics/Shaders/Context.hpp>

namespace Raindrop::Graphics::Shaders{
	GraphicsPipeline::GraphicsPipeline(Context& context, VkGraphicsPipelineCreateInfo info, VkPipelineLayoutCreateInfo layoutInfo, std::vector<std::shared_ptr<Shader>> shaders, std::string name) :
		_context{context},
		_shaders{shaders},
		_name{name}{
		
		auto& device = _context.graphics().internal().device();
		auto& allocationCallbacks = _context.graphics().allocationCallbacks();

		_pipeline = VK_NULL_HANDLE;
		_pipelineLayout = VK_NULL_HANDLE;

		if (vkCreatePipelineLayout(device.get(), &layoutInfo, allocationCallbacks, &_pipelineLayout) != VK_SUCCESS){
			throw std::runtime_error("Failed to create pipeline layout");
		}

		if (vkCreateGraphicsPipelines(device.get(), VK_NULL_HANDLE, 1, &info, allocationCallbacks, &_pipeline) != VK_SUCCESS){
			throw std::runtime_error("Failed to create graphics pipeline");
		}
	}

	GraphicsPipeline::~GraphicsPipeline(){
		auto& device = _context.graphics().internal().device();
		auto& allocationCallbacks = _context.graphics().allocationCallbacks();

		if (_pipeline) vkDestroyPipeline(device.get(), _pipeline, allocationCallbacks);
		if (_pipelineLayout) vkDestroyPipelineLayout(device.get(), _pipelineLayout, allocationCallbacks);
	}

	void GraphicsPipeline::bind(VkCommandBuffer commandBuffer){
		vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, _pipeline);
	}
	
	VkPipeline GraphicsPipeline::pipeline() const{
		return _pipeline;
	}

	VkPipelineLayout GraphicsPipeline::layout() const{
		return _pipelineLayout;
	}

	const std::string& GraphicsPipeline::name() const{
		return _name;
	}
}