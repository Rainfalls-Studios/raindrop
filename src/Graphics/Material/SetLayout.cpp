#include <Raindrop/Graphics/Materials/SetLayout.hpp>
#include <Raindrop/Graphics/Materials/Context.hpp>

#include <spdlog/spdlog.h>

#include <Raindrop/Exceptions/VulkanExceptions.hpp>

namespace Raindrop::Graphics::Materials{
	SetLayout::SetLayout(Context& context) : 
			_context{context},
			_setLayout{VK_NULL_HANDLE}
	{
		createSetLayout();
	}

	SetLayout::~SetLayout(){
		auto& device = _context.renderer.device;
		auto& allocationCallbacks = _context.renderer.allocationCallbacks;
		
		if (_setLayout != VK_NULL_HANDLE){
			vkDestroyDescriptorSetLayout(device.get(), _setLayout, allocationCallbacks);
			_setLayout = VK_NULL_HANDLE;
		}
	}

	VkDescriptorSetLayout SetLayout::get(){
		return _setLayout;
	}

	void SetLayout::createSetLayout(){
		auto& device = _context.renderer.device;
		auto& allocationCallbacks = _context.renderer.allocationCallbacks;

		VkDescriptorSetLayoutBinding bindings[] = {
			{
				.binding = 0,
				.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
				.descriptorCount = 1,
				.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT
			},
			{
				.binding = 1,
				.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
				.descriptorCount = 1,
				.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT
			}
		};

		VkDescriptorSetLayoutCreateInfo info{};
		info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		info.bindingCount = sizeof(bindings) / sizeof(bindings[0]);
		info.pBindings = bindings;

		Exceptions::checkVkCreation<VkDescriptorSetLayout>(
			vkCreateDescriptorSetLayout(device.get(), &info, allocationCallbacks, &_setLayout),
			"Failed to create descriptor set layout"
		);
	}
}

