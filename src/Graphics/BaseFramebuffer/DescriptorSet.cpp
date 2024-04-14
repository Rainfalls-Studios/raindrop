#include <Raindrop/Graphics/BaseFramebuffer/DescriptorSet.hpp>
#include <Raindrop/Graphics/BaseFramebuffer/Context.hpp>
#include <Raindrop/Graphics/Context.hpp>

#include <spdlog/spdlog.h>

#include <Raindrop/Exceptions/VulkanExceptions.hpp>

namespace Raindrop::Graphics::BaseFramebuffer{
	DescriptorSet::DescriptorSet(Context& context) : 
		_context{context},
		_set{VK_NULL_HANDLE},
		_layout{VK_NULL_HANDLE},
		_pool{VK_NULL_HANDLE}
	{
		spdlog::info("Constructing baseframebuffer descriptor set ...");

		createLayout();
		createPool();
		createSet();
	}

	DescriptorSet::~DescriptorSet(){
		spdlog::info("Destroying baseframebuffer descriptor set ...");

		destroySet();
		destroyPool();
		destroyLayout();
	}

	void DescriptorSet::createSet(){
		VkDescriptorSetAllocateInfo info{};
		info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		info.descriptorPool = _pool;
		info.descriptorSetCount = 1;
		info.pSetLayouts = &_layout;

		auto& device = _context.renderer.device;
		auto& allocationCallbacks = _context.renderer.allocationCallbacks;
		
		Exceptions::checkVkCreation<VkDescriptorSet>(
			vkAllocateDescriptorSets(device.get(), &info, &_set),
			"Failed to allocate base framebuffer descriptor set"
		);
	}

	void DescriptorSet::destroySet(){
		auto& device = _context.renderer.device;
		auto& allocationCallbacks = _context.renderer.allocationCallbacks;

		// Handled by the pool

		// if (_set != VK_NULL_HANDLE){
		// 	vkFreeDescriptorSets(device.get(), _pool, 1, &_set);
		// 	_set = VK_NULL_HANDLE;
		// }
	}

	void DescriptorSet::createPool(){
		VkDescriptorPoolSize poolSize{};
		poolSize.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		poolSize.descriptorCount = 1;

		VkDescriptorPoolCreateInfo info{};
		info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		info.maxSets = 1;
		info.pPoolSizes = &poolSize;
		info.poolSizeCount = 1;

		auto& device = _context.renderer.device;
		auto& allocationCallbacks = _context.renderer.allocationCallbacks;

		Exceptions::checkVkCreation<VkDescriptorPool>(
			vkCreateDescriptorPool(device.get(), &info, allocationCallbacks, &_pool),
			"Failed to create base framebuffer descriptor pool"
		);
	}

	void DescriptorSet::destroyPool(){
		auto& device = _context.renderer.device;
		auto& allocationCallbacks = _context.renderer.allocationCallbacks;
		
		Exceptions::checkVkOperation<VkDescriptorPool>(
			vkResetDescriptorPool(device.get(), _pool, 0),
			"Failed to reset base framebuffer descriptor pool",
			Exceptions::VulkanOperationType::RESET
		);
		
		if (_pool != VK_NULL_HANDLE){
			vkDestroyDescriptorPool(device.get(), _pool, allocationCallbacks);
			_pool = VK_NULL_HANDLE;
		}
	}

	void DescriptorSet::createLayout(){
		VkDescriptorSetLayoutBinding binding{};
		binding.binding = 0;
		binding.descriptorCount = 1;
		binding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		binding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

		VkDescriptorSetLayoutCreateInfo info{};
		info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		info.bindingCount = 1;
		info.pBindings = &binding;

		auto& device = _context.renderer.device;
		auto& allocationCallbacks = _context.renderer.allocationCallbacks;

		Exceptions::checkVkCreation<VkDescriptorSetLayout>(
			vkCreateDescriptorSetLayout(device.get(), &info, allocationCallbacks, &_layout),
			"Failed to create base framebuffer descriptor set layout"
		);
	}

	void DescriptorSet::destroyLayout(){
		auto& device = _context.renderer.device;
		auto& allocationCallbacks = _context.renderer.allocationCallbacks;

		if (_layout != VK_NULL_HANDLE){
			vkDestroyDescriptorSetLayout(device.get(), _layout, allocationCallbacks);
			_layout = VK_NULL_HANDLE;
		}
	}

	VkDescriptorSet DescriptorSet::set() const{
		return _set;
	}

	VkDescriptorSetLayout DescriptorSet::layout() const{
		return _layout;
	}

	VkDescriptorPool DescriptorSet::pool() const{
		return _pool;
	}

	void DescriptorSet::update(VkImageView image){
		auto& device = _context.renderer.device;
		auto& allocationCallbacks = _context.renderer.allocationCallbacks;

		VkDescriptorImageInfo imageInfo{};
		imageInfo.sampler = _context.sampler.get();
		imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		imageInfo.imageView = image;

		VkWriteDescriptorSet write{};
		write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		write.descriptorCount = 1;
		write.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		write.pImageInfo = &imageInfo;
		write.dstSet = _set;
		write.dstBinding = 0;

		vkUpdateDescriptorSets(device.get(), 1, &write, 0, nullptr);
	}
}