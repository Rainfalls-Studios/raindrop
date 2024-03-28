#include <Raindrop/Renderer/Queues/Manager.hpp>
#include <Raindrop/Renderer/Context.hpp>
#include <spdlog/spdlog.h>

namespace Raindrop::Renderer::Queues{
	Manager::Manager(Context& context) :
			_context{context},
			_graphicsFamily{INVALID_FAMILY_INDEX},
			_presentFamily{INVALID_FAMILY_INDEX},
			_transfertFamily{INVALID_FAMILY_INDEX},
			_graphicsQueue{VK_NULL_HANDLE},
			_presentQueue{VK_NULL_HANDLE},
			_transfertQueue{VK_NULL_HANDLE}
		{

		spdlog::info("Constructing vulkan queue manager ...");
		auto queueFamilies = context.physicalDevice.queueFamilyProperties();

		findGraphicsFamily(queueFamilies);
		findPresentFamily(queueFamilies);
		findTransfertFamily(queueFamilies);
		getQueues(queueFamilies);
	}

	Manager::~Manager(){
		spdlog::info("Destroying vulkan queue manager ...");
	}

	void Manager::findGraphicsFamily(std::vector<VkQueueFamilyProperties>& families){
		spdlog::trace("Looking for graphics queue family ...");

		for (std::size_t i=0; i<families.size(); i++){
			auto &family = families[i];

			if (family.queueFlags & VK_QUEUE_GRAPHICS_BIT){
				_graphicsFamily = i;
				return;
			}
		}

		throw std::runtime_error("Failed to find a queue with graphics support");
	}

	void Manager::findPresentFamily(std::vector<VkQueueFamilyProperties>& families){
		spdlog::trace("Looking for present queue family ...");
		{
			VkBool32 supported = VK_FALSE;
			vkGetPhysicalDeviceSurfaceSupportKHR(_context.physicalDevice.get(), _graphicsFamily, _context.window.surface(), &supported);
			if (supported){
				_presentFamily = _graphicsFamily;
				return;
			} 
		}

		for (std::size_t i=0; i<families.size(); i++){
			VkBool32 presentSupport = VK_FALSE;
			if (vkGetPhysicalDeviceSurfaceSupportKHR(_context.physicalDevice.get(), i, _context.window.surface(), &presentSupport) != VK_SUCCESS){
				throw std::runtime_error("Failed to query queue physical device support");
			}

			if (presentSupport == VK_TRUE){
				_presentFamily = i;
				return;
			}
		}

		throw std::runtime_error("Failed to find a queue with present support");
	}

	void Manager::findTransfertFamily(std::vector<VkQueueFamilyProperties>& families){
		spdlog::trace("Looking for transfert queue family ...");
		// tries to use the same family as the graphics family
		if (families[_graphicsFamily].queueFlags & VK_QUEUE_TRANSFER_BIT){
			_transfertFamily = _graphicsFamily;
			return;
		}

		for (std::size_t i=0; i<families.size(); i++){
			auto& family = families[i];

			if (family.queueFlags & VK_QUEUE_TRANSFER_BIT == 0) continue;

			_transfertFamily = i;
			return;
		}

		throw std::runtime_error("Failed to find a queue with transfert support");
	}

	void Manager::getQueues(std::vector<VkQueueFamilyProperties>& families){
		auto device = _context.device.get();

		// if all three queues are the same
		if (_graphicsFamily == _presentFamily && _transfertFamily == _presentFamily){
			switch (families[_graphicsFamily].queueCount){
				case 1:{
					vkGetDeviceQueue(device, _graphicsFamily, 0, &_graphicsQueue);
					_presentQueue = _graphicsQueue;
					_transfertQueue = _graphicsQueue;
					break;
				}

				case 2:{
					vkGetDeviceQueue(device, _graphicsFamily, 0, &_graphicsQueue);
					_presentQueue = _graphicsQueue;

					vkGetDeviceQueue(device, _graphicsFamily, 1, &_transfertQueue);
					break;
				}

				default:{
					vkGetDeviceQueue(device, _graphicsFamily, 0, &_graphicsQueue);
					vkGetDeviceQueue(device, _graphicsFamily, 1, &_presentQueue);
					vkGetDeviceQueue(device, _graphicsFamily, 2, &_transfertQueue);
					break;
				}
			}

		} else if (_graphicsFamily == _presentFamily){
			if (families[_graphicsFamily].queueCount >= 2){
				vkGetDeviceQueue(device, _graphicsFamily, 0, &_graphicsQueue);
				vkGetDeviceQueue(device, _graphicsFamily, 1, &_presentQueue);
			} else {
				vkGetDeviceQueue(device, _graphicsFamily, 0, &_graphicsQueue);
				_presentQueue = _graphicsQueue;
			}

			vkGetDeviceQueue(device, _transfertFamily, 0, &_transfertQueue);
			
		} else if (_graphicsFamily == _transfertFamily){

			if (families[_graphicsFamily].queueCount >= 2){
				vkGetDeviceQueue(device, _graphicsFamily, 0, &_graphicsQueue);
				vkGetDeviceQueue(device, _graphicsFamily, 1, &_transfertQueue);
			} else {
				vkGetDeviceQueue(device, _graphicsFamily, 0, &_graphicsQueue);
				_transfertQueue = _graphicsQueue;
			}

			vkGetDeviceQueue(device, _presentFamily, 0, &_presentQueue);

		} else {
			if (families[_presentFamily].queueCount >= 2){
				vkGetDeviceQueue(device, _presentFamily, 0, &_presentQueue);
				vkGetDeviceQueue(device, _presentFamily, 1, &_transfertQueue);
			} else {
				vkGetDeviceQueue(device, _presentFamily, 0, &_presentQueue);
				_transfertQueue = _presentQueue;
			}

			vkGetDeviceQueue(device, _graphicsFamily, 0, &_graphicsQueue);
		}
	}

	VkQueue Manager::graphicsQueue() const{
		return _graphicsQueue;
	}

	VkQueue Manager::presentQueue() const{
		return _presentQueue;
	}

	VkQueue Manager::transfertQueue() const{
		return _transfertQueue;
	}

	std::size_t Manager::graphicsFamily() const{
		return _graphicsFamily;
	}

	std::size_t Manager::presentFamily() const{
		return _presentFamily;
	}

	std::size_t Manager::transfertFamily() const{
		return _transfertFamily;
	}
}