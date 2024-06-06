#include <Raindrop_internal/Graphics/PipelineLayout.hpp>
#include <Raindrop_internal/Graphics/Context.hpp>

#include <spdlog/spdlog.h>

#include <Raindrop/Exceptions/VulkanExceptions.hpp>

namespace Raindrop::Internal::Graphics{
	PipelineLayout::PipelineLayout(Context& context, const PipelineLayoutConfigInfo& info) :
			_context{context},
			_layout{VK_NULL_HANDLE}{
		createLayout(info);
	}

	PipelineLayout::~PipelineLayout(){
		destroyLayout();
	}

	void PipelineLayout::createLayout(const PipelineLayoutConfigInfo& info){
		_context.getLogger()->info("Creating a pipeline layout ...");

		auto& device = _context.getDevice();
		auto& allocationCallbacks = _context.getAllocationCallbacks();

		VkPipelineLayoutCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;

		if (info.pushConstant.has_value()){
			createInfo.pushConstantRangeCount = 1;
			createInfo.pPushConstantRanges = &info.pushConstant.value();
		} else {
			createInfo.pushConstantRangeCount = 0;
		}

		createInfo.setLayoutCount = static_cast<uint32_t>(info.setLayouts.size());
		createInfo.pSetLayouts = info.setLayouts.data();

		Exceptions::checkVkCreation<VkPipelineLayout>(
			vkCreatePipelineLayout(device.get(), &createInfo, allocationCallbacks, &_layout),
			"Failed to create pipeline layout"
		);
	}

	void PipelineLayout::destroyLayout(){
		auto& device = _context.getDevice();
		auto& allocationCallbacks = _context.getAllocationCallbacks();

		if (_layout != VK_NULL_HANDLE){
			vkDestroyPipelineLayout(device.get(), _layout, allocationCallbacks);
			_layout = VK_NULL_HANDLE;	
		}
	}

	VkPipelineLayout PipelineLayout::get() const noexcept{
		return _layout;
	}
}