#include "Raindrop/Graphics/Vulkan/Device.hpp"
#include "Raindrop/Graphics/Context.hpp"

namespace Raindrop::Graphics::Vulkan{
	Device::Device(Context& context) : 
		_context{context},
		_device{}
	{
		SPDLOG_LOGGER_INFO(_context.getLogger(), "Creating vulkan device...");

		vkb::DeviceBuilder builder(_context.getPhysicalDevice().getVkb());

		vkb::Result result = builder.build();
		if (!result){
			SPDLOG_LOGGER_ERROR(_context.getLogger(), "Failed to create vulkan device : {}", result.error().message());
			throw std::runtime_error("Failed to create vulkan device");
		}

		_device = *result;

		getQueues();

		SPDLOG_LOGGER_INFO(_context.getLogger(), "Vulkan device created with success !");
	}
	
	Device::~Device(){
		vkb::destroy_device(_device);
	}
	
	const VkDevice& Device::get() const noexcept{
		return _device.device;
	}

	const vkb::Device& Device::getVkb() const noexcept{
		return _device;
	}

	constexpr const char* queueTypeToStr(vkb::QueueType type){
		switch (type){
			case vkb::QueueType::present: return "present";
			case vkb::QueueType::graphics: return "graphics";
			case vkb::QueueType::compute: return "compute";
			case vkb::QueueType::transfer: return "transfer";
		}
		throw std::runtime_error("invalid queue");
	}

	void Device::getQueues(){
		auto initQueue = [this](const vkb::QueueType& type) -> QueueInfo {

			auto queue = _device.get_queue(type);
			if (!queue){
				SPDLOG_LOGGER_ERROR(_context.getLogger(), "Couldn't find {} queue type : {}", queueTypeToStr(type), queue.error().message());
				throw std::runtime_error("Couldn't find queue");
			}

			return QueueInfo{ 
					.queue = queue.value(),
					.familyIndex = _device.get_queue_index(type).value()
				}; 
		};

		graphicsQueue = initQueue(vkb::QueueType::graphics);
		presentQueue = initQueue(vkb::QueueType::present);
		transfetQueue = initQueue(vkb::QueueType::transfer);
	}

	void Device::waitIdle(){
		vkDeviceWaitIdle(_device.device);
	}
}