// #include <Raindrop/Graphics/GlobalDescriptorPool.hpp>
// #include <Raindrop/Graphics/GraphicsContext.hpp>

// namespace Raindrop::Graphics{
// 	GlobalDescriptorPool::GlobalDescriptorPool(GraphicsContext& context) :
// 		_context{context},
// 		_pool{VK_NULL_HANDLE}{
// 		createPool();
// 	}

// 	GlobalDescriptorPool::~GlobalDescriptorPool(){
// 		destroyPool();
// 	}
	
// 	void GlobalDescriptorPool::createPool(){
// 		destroyPool();

// 		auto device = _context.device.get();

// 		VkDescriptorPoolSize pool_sizes[] = {
// 			{VK_DESCRIPTOR_TYPE_SAMPLER, 1000},
// 			{VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000},
// 			{VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000},
// 			{VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000},
// 			{VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000},
// 			{VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000},
// 			{VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000},
// 			{VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000},
// 			{VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000},
// 			{VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000},
// 			{VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000}
// 		};

// 		VkDescriptorPoolCreateInfo pool_info = {};
// 		pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
// 		pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
// 		pool_info.maxSets = 1000;
// 		pool_info.poolSizeCount = std::size(pool_sizes);
// 		pool_info.pPoolSizes = pool_sizes;

// 		if (vkCreateDescriptorPool(device, &pool_info, nullptr, &_pool) != VK_SUCCESS){
// 			throw std::runtime_error("failed to create global descriptor pool");
// 		}
// 	}

// 	void GlobalDescriptorPool::destroyPool(){
// 		auto device = _context.device.get();
// 		auto allocationCallbacks = _context.allocationCallbacks;

// 		if (_pool != VK_NULL_HANDLE) vkDestroyDescriptorPool(device, _pool, allocationCallbacks);
// 		_pool = VK_NULL_HANDLE;
// 	}

// 	VkDescriptorPool GlobalDescriptorPool::get() const{
// 		return _pool;
// 	}
// }