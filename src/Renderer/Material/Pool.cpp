#include <Raindrop/Renderer/Material/Pool.hpp>
#include <Raindrop/Renderer/Material/Context.hpp>

#include <spdlog/spdlog.h>


namespace Raindrop::Renderer::Material{
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
				.descriptorCount = static_cast<uint32_t>(instanceCount)
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

		if (vkCreateDescriptorPool(device.get(), &info, allocationCallbacks, &_pool) != VK_SUCCESS){
			spdlog::error("Failed to create material descriptor pool (size : {})", instanceCount);
			throw std::runtime_error("Failed to create descriptor pool");
		}
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

		if (vkAllocateDescriptorSets(device.get(), &info, _descriptorSets.data()) != VK_SUCCESS){
			spdlog::error("Failed to allocate material descriptor sets (count : {})", instanceCount);
			throw std::runtime_error("Failed to allocate material descriptor sets");
		}
	}

	VkDescriptorPool Pool::get() const{
		return _pool;
	}

	VkDescriptorSet Pool::getSet(const std::size_t& index) const{
		assert(index < _descriptorSets.size() && "Invalid material descriptor set index");
		return _descriptorSets[index];
	}
}