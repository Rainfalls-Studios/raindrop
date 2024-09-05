// #include <Raindrop/Graphics/CommandPool.hpp>
// #include <Raindrop/Graphics/Context.hpp>
// #include <Raindrop/Exceptions/VulkanExceptions.hpp>
// #include <Raindrop/Graphics/CommandBuffer.hpp>
// #include <Raindrop/Context.hpp>

// namespace Raindrop::Graphics::Vulkan::Core{
// 	std::shared_ptr<CommandPool> CommandPool::create(Raindrop::Context& context){
// 		return context.registry.emplace<CommandPool>();
// 	}

// 	CommandPool::CommandPool() : 
// 		_context{nullptr},
// 		_pool{VK_NULL_HANDLE}
// 	{}

// 	CommandPool::~CommandPool(){
// 		release();
// 	}

// 	CommandPool& CommandPool::prepare(Context& context){
// 		_context = &context;

// 		return *this;
// 	}

// 	void CommandPool::initialize(){
// 		_context->logger->info("Creating command pool...");


// 		VkCommandPoolCreateInfo info{};
// 		info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
// 		info.flags = _flags;
// 		info.queueFamilyIndex = static_cast<uint32_t>(_familyIndex);

// 		auto& device = _context->core.device;
// 		auto& allocationCallbacks = _context->core.allocationCallbacks;

// 		Exceptions::checkVkCreation<VkCommandPool>(
// 			vkCreateCommandPool(device.get(), &info, allocationCallbacks, &_pool),
// 			"Failed to create command pool",
// 			_context->logger
// 		);
// 	}

// 	void CommandPool::release(){
// 		if (!_context) return;

// 		auto& device = _context->core.device;
// 		auto& allocationCallbacks = _context->core.allocationCallbacks;

// 		if (_pool != VK_NULL_HANDLE){
// 			_context->logger->info("Destroying command pool...");
// 			vkDestroyCommandPool(device.get(), _pool, allocationCallbacks);
// 			_pool = VK_NULL_HANDLE;
// 		}

// 		_context = nullptr;
// 	}

// 	const VkCommandPool& CommandPool::get() const noexcept{
// 		return _pool;
// 	}

// 	CommandPool& CommandPool::setCreateFlags(const VkCommandPoolCreateFlags& flags){
// 		if (_pool != VK_NULL_HANDLE) throw std::runtime_error("Cannot change build properties when the pool has already been initialied");
// 		_flags = flags;
// 		return *this;
// 	}

// 	CommandPool& CommandPool::setQueueFamily(const std::uint32_t& index){
// 		if (_pool != VK_NULL_HANDLE) throw std::runtime_error("Cannot change build properties when the pool has already been initialied");
// 		_familyIndex = index;
// 		return *this;
// 	}

// 	std::vector<CommandBuffer> CommandPool::allocate(const std::uint32_t& count, const VkCommandBufferLevel& level){
// 		_context->logger->info("Allocating {} command buffers...", count);

// 		VkCommandBufferAllocateInfo info{};
// 		info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
// 		info.commandPool = _pool;
// 		info.commandBufferCount = static_cast<uint32_t>(count);
// 		info.level = level;

// 		std::vector<VkCommandBuffer> vkBuffers(static_cast<std::size_t>(count));
// 		auto& device = _context->getDevice();
		
// 		Exceptions::checkVkOperation<VkCommandPool>(
// 			vkAllocateCommandBuffers(device.get(), &info, vkBuffers.data()),
// 			"Failed to allocate command buffers",
// 			Exceptions::VulkanOperationType::ALLOCATION,
// 			_context->logger
// 		);

// 		std::vector<CommandBuffer> output;
// 		std::transform(
// 			vkBuffers.begin(),
// 			vkBuffers.end(),
// 			std::back_inserter(output),

// 			[this](const VkCommandBuffer& commandBuffer) {
// 				return CommandBuffer(*_context, commandBuffer);
// 			}
// 		);

// 		return output;
// 	}

// 	void CommandPool::free(const std::vector<CommandBuffer>& commandBuffers){
// 		_context->logger->info("Freeing {} command buffers...", commandBuffers.size());
// 		auto& device = _context->getDevice();

// 		std::vector<VkCommandBuffer> vkCommandBuffers(commandBuffers.size());
// 		for (std::size_t i=0; i<commandBuffers.size(); i++){
// 			vkCommandBuffers[i] = commandBuffers[i].get();
// 		}

// 		vkFreeCommandBuffers(device.get(), _pool, static_cast<uint32_t>(commandBuffers.size()), vkCommandBuffers.data());
// 	}

// 	void CommandPool::reset(const VkCommandPoolResetFlags& flags){
// 		auto& device = _context->getDevice();

// 		Exceptions::checkVkOperation<VkCommandPool>(
// 			vkResetCommandPool(device.get(), _pool, flags),
// 			"Failed to reset command pool",
// 			Exceptions::VulkanOperationType::RESET,
// 			_context->logger
// 		);
// 	}

// 	void CommandPool::trim(const VkCommandPoolTrimFlags& flags){
// 		auto& device = _context->getDevice();

// 		vkTrimCommandPool(device.get(), _pool, flags);
// 	}
// }