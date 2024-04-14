#include <Raindrop/Graphics/Materials/Pool.hpp>
#include <Raindrop/Graphics/Materials/Context.hpp>

#include <spdlog/spdlog.h>

#include <Raindrop/Exceptions/VulkanExceptions.hpp>

namespace Raindrop::Graphics::Materials{
	Pool::Pool(Context& context) : 
			_context{context},
			_pool{VK_NULL_HANDLE}
	{
		createPool();
		allocateDescriptors();
	}

	Pool::~Pool(){
		auto& device = _context.renderer.device;
		auto& allocationCallbacks = _context.renderer.allocationCallbacks;

		if (_pool != VK_NULL_HANDLE){
			vkDestroyDescriptorPool(device.get(), _pool, allocationCallbacks);
			_pool = VK_NULL_HANDLE;
		}
	}

	void Pool::createPool(){
		auto& instanceCount = _context.instanceCount;
		auto& device = _context.renderer.device;
		auto& allocationCallbacks = _context.renderer.allocationCallbacks;
		VkDescriptorPoolSize sizes[] = {
			{
				.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
				.descriptorCount = static_cast<uint32_t>(instanceCount) * 2
			},
			{
				.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
				.descriptorCount = static_cast<uint32_t>(instanceCount)
			}
		};

		VkDescriptorPoolCreateInfo info{};
		info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		info.maxSets = static_cast<uint32_t>(instanceCount);
		info.pPoolSizes = sizes;
		info.poolSizeCount = sizeof(sizes) / sizeof(sizes[0]);

		Exceptions::checkVkCreation<VkDescriptorPool>(
			vkCreateDescriptorPool(device.get(), &info, allocationCallbacks, &_pool),
			"Failed to create material descriptor pool"
		);
	}

	void Pool::allocateDescriptors(){
		auto& instanceCount = _context.instanceCount;
		auto& device = _context.renderer.device;
		
		_descriptorSets.resize(instanceCount);
		std::vector<VkDescriptorSetLayout> setLayouts(instanceCount);
		std::fill(setLayouts.begin(), setLayouts.end(), _context.layout.get());

		VkDescriptorSetAllocateInfo info{};
		info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		info.descriptorPool = _pool;
		info.descriptorSetCount = static_cast<uint32_t>(instanceCount);
		info.pSetLayouts = setLayouts.data();

		Exceptions::checkVkCreation<VkDescriptorSet>(
			vkAllocateDescriptorSets(device.get(), &info, _descriptorSets.data()),
			"Failed to allocate material descriptor sets"
		);
	}

	VkDescriptorPool Pool::get() const{
		return _pool;
	}

	VkDescriptorSet Pool::getSet(const std::size_t& index) const{
		assert(index < _descriptorSets.size() && "Invalid material descriptor set index");
		return _descriptorSets[index];
	}
}