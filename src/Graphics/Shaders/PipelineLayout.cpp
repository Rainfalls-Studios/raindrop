#include <Raindrop/Graphics/Shaders/PipelineLayout.hpp>
#include <Raindrop/Graphics/Shaders/PipelineLayoutBuilder.hpp>
#include <Raindrop/Graphics/Shaders/Context.hpp>

namespace Raindrop::Graphics::Shaders{
	PipelineLayout::PipelineLayout(Context& context, const PipelineLayoutBuilder& builder) : _context{context}{
		_layout = VK_NULL_HANDLE;

		VkPipelineLayoutCreateInfo info{};
		info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		info.flags = builder.flags();
		info.pushConstantRangeCount = builder.pushConstantCount();
		info.pPushConstantRanges = builder.pushConstants();
		info.setLayoutCount = builder.setLayoutCount();
		info.pSetLayouts = builder.setLayouts();

		auto& device = _context.graphics().internal().device();
		auto& allocationCallbacks = _context.graphics().allocationCallbacks();

		if (vkCreatePipelineLayout(device.get(), &info, allocationCallbacks, &_layout) != VK_SUCCESS){
			_context.logger().info("Failed to create pipeline layout \"{}\"", builder.name());
			throw std::runtime_error("Failed to create pipeline layout");
		}
	}

	PipelineLayout::~PipelineLayout(){
		auto& device = _context.graphics().internal().device();
		auto& allocationCallbacks = _context.graphics().allocationCallbacks();

		if (_layout != VK_NULL_HANDLE){
			vkDestroyPipelineLayout(device.get(), _layout, allocationCallbacks);
			_layout = VK_NULL_HANDLE;
		}
	}

	VkPipelineLayout PipelineLayout::get() const{
		return _layout;
	}
}