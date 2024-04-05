#include <Raindrop/Graphics/Pipelines/PipelineLayout.hpp>
#include <Raindrop/Graphics/Context.hpp>

#include <spdlog/spdlog.h>

namespace Raindrop::Graphics::Pipelines{
	PipelineLayout::PipelineLayout(Context& context, const PipelineLayoutConfigInfo& info) :
			_context{context},
			_layout{VK_NULL_HANDLE}{
		createLayout(info);
	}

	PipelineLayout::~PipelineLayout(){
		destroyLayout();
	}

	void PipelineLayout::createLayout(const PipelineLayoutConfigInfo& info){
		spdlog::info("Creating a pipeline layout ...");
		auto& device = _context.device;
		auto& allocationCallbacks = _context.allocationCallbacks;

		VkPipelineLayoutCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;

		createInfo.pushConstantRangeCount = static_cast<uint32_t>(info.pushConstants.size());
		createInfo.pPushConstantRanges = info.pushConstants.data();

		createInfo.setLayoutCount = static_cast<uint32_t>(info.setLayouts.size());
		createInfo.pSetLayouts = info.setLayouts.data();

		if (vkCreatePipelineLayout(device.get(), &createInfo, allocationCallbacks, &_layout) != VK_SUCCESS){
			spdlog::error("Failed to create a pipeline layout");
			throw std::runtime_error("Failed to create pipeline layout");
		}
	}

	void PipelineLayout::destroyLayout(){
		auto& device = _context.device;
		auto& allocationCallbacks = _context.allocationCallbacks;

		if (_layout != VK_NULL_HANDLE){
			vkDestroyPipelineLayout(device.get(), _layout, allocationCallbacks);
			_layout = VK_NULL_HANDLE;	
		}
	}

	VkPipelineLayout PipelineLayout::get() const noexcept{
		return _layout;
	}
}