// #include "Raindrop/Graphics/Vulkan/Device.hpp"
// #include "Raindrop/Graphics/Context.hpp"

// namespace Raindrop::Graphics::Vulkan{
// 	Device::Device() noexcept : 
// 		_context{nullptr},
// 		_device{},
// 		_builder{}
// 	{}
	
// 	Device::~Device(){
// 		release();	
// 	}

// 	Device& Device::prepare(Context& context){

// 		// Store the context and make the builder
// 		_context = &context;
// 		_builder = std::make_unique<vkb::DeviceBuilder>(context.getPhysicalDevice().getVkb());

// 		return *this;
// 	}

// 	void Device::initialize(){
// 		auto result = _builder->build();
// 		SPDLOG_LOGGER_INFO(_context->getLogger(), "Creating vulkan device...");

// 		// If vkb didn't managed to create a valide device, throw and error
// 		if (!result){
// 			SPDLOG_LOGGER_ERROR(_context->getLogger(), "Failed to create vulkan device : {}", result.error().message());
// 			throw std::runtime_error("Failed to create vulkan device");
// 		}

// 		// Store the device and reset the builder to free some space
// 		_device = result.value();
// 		_builder.reset();

// 		// checkQueuePresence();
// 		getQueues();
// 	}

// 	void Device::release(){
// 		SPDLOG_LOGGER_INFO(_context->getLogger(), "Destroying vulkan device...");
// 		_builder.reset();
// 		vkb::destroy_device(_device);
// 		_context = nullptr;
// 	}

	
// 	const VkDevice& Device::get() const noexcept{
// 		return _device.device;
// 	}

// 	const vkb::Device& Device::getVkb() const noexcept{
// 		return _device;
// 	}

// 	// void Device::checkQueuePresence(){
// 	// 	// Shouldn't append since this check is already made in the physical device selection. But it's not expensive to check twice


// 	// 	auto graphics = _device.get_queue_index(vkb::QueueType::graphics);

// 	// 	if (!graphics){
// 	// 		SPDLOG_LOGGER_ERROR(_context->getLogger(), "The device does not support graphics queues");
// 	// 		throw std::runtime_error("The device does not support graphics queues");
// 	// 	}

// 	// 	// TODO: change that
// 	// 	const bool surfaceRequired = _context->physicalDevice.getVkb().surface != VK_NULL_HANDLE;

// 	// 	if (surfaceRequired){
// 	// 		auto present = _device.get_queue_index(vkb::QueueType::present);

// 	// 		if (!present){		
// 	// 			SPDLOG_LOGGER_ERROR(_context->getLogger(), "The device does not support graphics queues");
// 	// 			_context->logger->critical("The device does not support present queues");
// 	// 			throw std::runtime_error("The device does not support present queues");
// 	// 		}

// 	// 		if (present.value() != graphics.value()){
// 	// 			_context->logger->warn("The main grahics queue does not share the same family has the present queue : {} != {}", graphics.value(), present.value());
// 	// 		}
// 	// 	}

// 	// 	auto transfert = _device.get_queue_index(vkb::QueueType::transfer);
// 	// 	if (!transfert){
// 	// 		_context->logger->critical("The device does not support transfert queues");
// 	// 		throw std::runtime_error("The device does not support transfert queues");
// 	// 	}

// 	// 	if (transfert.value() != graphics.value()){
// 	// 		_context->logger->warn("The transfert queue does not share the same family has the graphics queue : {} != {}", transfert.value(), graphics.value());
// 	// 	}
// 	// }

// 	void Device::getQueues(){
// 		auto initQueue = [this](const vkb::QueueType& type) -> QueueInfo {
// 			// We can skip queue check because we already did that
// 			return QueueInfo{ 
// 					.queue = _device.get_queue(type).value(),
// 					.familyIndex = _device.get_queue_index(type).value()
// 				};
// 		};

// 		graphicsQueue = initQueue(vkb::QueueType::graphics);
// 		presentQueue = initQueue(vkb::QueueType::present);
// 		transfetQueue = initQueue(vkb::QueueType::transfer);
// 	}

// 	void Device::waitIdle(){
// 		vkDeviceWaitIdle(_device.device);
// 	}

// 	uint32_t Device::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties){
// 		auto& physicalDevice = _context->getPhysicalDevice();

// 		VkPhysicalDeviceMemoryProperties memProperties;
// 		vkGetPhysicalDeviceMemoryProperties(physicalDevice.get(), &memProperties);

// 		for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
// 			if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
// 				return i;
// 			}
// 		}

// 		SPDLOG_LOGGER_WARN(_context->getLogger(), "Failed to find a suitable memory type (filter : {}, properties : {})", typeFilter, properties);
// 		throw std::runtime_error("failed to find suitable memory type");
// 	}
// }