#include <Raindrop/Graphics/Core/Device.hpp>
#include <Raindrop/Graphics/Context.hpp>

#include <spdlog/spdlog.h>

#include <Raindrop/Exceptions/VulkanExceptions.hpp>

namespace Raindrop::Graphics::Core{
	Device::Device() noexcept : 
		_context{nullptr},
		_device{},
		_builder{}
	{}
	
	Device::~Device(){
		release();	
	}

	void Device::prepare(Context& context){
		_context = &context;
		_builder = std::make_unique<vkb::DeviceBuilder>(context.physicalDevice.getVkb());
	}

	void Device::initialize(){
		auto result = _builder->build();

		_context->logger->info("Creating vulkan device...");

		if (!result){
			_context->logger->error("Failed to create vulkan device : {}", result.error().message());
			throw std::runtime_error("Failed to create vulkan device");
		}

		_device = result.value();
		_builder.reset();

		checkQueuePresence();
		getQueues();
	}

	void Device::release(){
		_context->logger->info("Destroying vulkan device...");
		_builder.reset();
		vkb::destroy_device(_device);
		_context = nullptr;
	}

	
	const VkDevice& Device::get() const noexcept{
		return _device.device;
	}

	const vkb::Device& Device::getVkb() const noexcept{
		return _device;
	}

	void Device::checkQueuePresence(){
		auto graphics = _device.get_queue_index(vkb::QueueType::graphics);

		if (!graphics){
			_context->logger->critical("The device does not support graphics queues");
			throw std::runtime_error("The device does not support graphics queues");
		}

		// TODO: change that
		const bool surfaceRequired = _context->physicalDevice.getVkb().surface != VK_NULL_HANDLE;

		if (surfaceRequired){
			auto present = _device.get_queue_index(vkb::QueueType::present);

			if (!present){
				_context->logger->critical("The device does not support present queues");
				throw std::runtime_error("The device does not support present queues");
			}

			if (present.value() != graphics.value()){
				_context->logger->warn("The main grahics queue does not share the same family has the present queue : {} != {}", graphics.value(), present.value());
			}
		}

		auto transfert = _device.get_queue_index(vkb::QueueType::transfer);
		if (!transfert){
			_context->logger->critical("The device does not support transfert queues");
			throw std::runtime_error("The device does not support transfert queues");
		}

		if (transfert.value() != graphics.value()){
			_context->logger->warn("The transfert queue does not share the same family has the graphics queue : {} != {}", transfert.value(), graphics.value());
		}
	}

	void Device::getQueues(){
		auto initQueue = [this](const vkb::QueueType& type) -> QueueInfo {
			// We can skip queue check because we already did that
			return QueueInfo{ 
					.queue = _device.get_queue(type).value(),
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