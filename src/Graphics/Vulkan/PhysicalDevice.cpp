#include "Raindrop/Graphics/Vulkan/PhysicalDevice.hpp"
#include "Raindrop/Graphics/Context.hpp"

namespace Raindrop::Graphics::Vulkan{
	PhysicalDevice::PhysicalDevice(Context& context) :
		_context{context},
		_device{}
	{
		vkb::PhysicalDeviceSelector selector(context.getInstance().getVkb());

		selector
			.set_surface(_context.getSurface().get())
			.require_present()
			.prefer_gpu_device_type(vkb::PreferredDeviceType::discrete)
			.allow_any_gpu_device_type(false);

		vkb::Result result = selector.select();

		// Strict device type selection at the begining
		if (!result){
			selector.allow_any_gpu_device_type();
			vkb::Result result = selector.select();

			// if the filter is too strict and no device has been found
			if (!result){
				SPDLOG_LOGGER_ERROR(_context.getLogger(), "Couldn't find a suitable physical device : {}", result.error().message());
				throw std::runtime_error("Couln't find a suitable physical device");
			}
		}

		_device = *result;
		SPDLOG_LOGGER_INFO(_context.getLogger(), "Found a physical device : {}", _device.name);
	}

	PhysicalDevice::~PhysicalDevice(){
		
	}

	const VkPhysicalDevice& PhysicalDevice::get() const noexcept{
		return _device.physical_device;
	}

	vkb::PhysicalDevice& PhysicalDevice::getVkb() noexcept{
		return _device;
	}

	const vkb::PhysicalDevice& PhysicalDevice::getVkb() const noexcept{
		return _device;
	}

	std::vector<VkQueueFamilyProperties> PhysicalDevice::getQueueFamilyProperties() const{
		return _device.get_queue_families();
	}
}