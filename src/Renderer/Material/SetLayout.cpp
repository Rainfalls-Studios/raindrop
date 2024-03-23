#include <Raindrop/Renderer/Material/SetLayout.hpp>
#include <Raindrop/Renderer/Context.hpp>

#include <spdlog/spdlog.h>

namespace Raindrop::Renderer::Material{
	SetLayout::SetLayout(Context& context) : 
			_context{context},
			_setLayout{VK_NULL_HANDLE}
	{
		createSetLayout();
	}

	SetLayout::~SetLayout(){
		auto& device = _context.device;
		auto& allocationCallbacks = _context.allocationCallbacks;
		
		if (_setLayout != VK_NULL_HANDLE){
			vkDestroyDescriptorSetLayout(device.get(), _setLayout, allocationCallbacks);
			_setLayout = VK_NULL_HANDLE;
		}
	}

	VkDescriptorSetLayout SetLayout::get(){
		return _setLayout;
	}

	void SetLayout::createSetLayout(){
		auto& device = _context.device;
		auto& allocationCallbacks = _context.allocationCallbacks;

		VkDescriptorSetLayoutBinding bindings[] = {
			VkDescriptorSetLayoutBinding{
				.binding = 0,
				.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
				.descriptorCount = 1,
				.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT
			}
		};

		VkDescriptorSetLayoutCreateInfo info{};
		info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		info.bindingCount = sizeof(bindings) / sizeof(bindings[0]);
		info.pBindings = bindings;
		
		if (vkCreateDescriptorSetLayout(device.get(), &info, allocationCallbacks, &_setLayout) != VK_SUCCESS){
			spdlog::error("Failed to create descriptor set layout");
			throw std::runtime_error("Failed to create descriptor set layout");
		}
	}
}