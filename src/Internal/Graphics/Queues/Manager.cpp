#include <Raindrop_internal/Graphics/Queues/Manager.hpp>
#include <Raindrop_internal/Graphics/Context.hpp>
#include <spdlog/spdlog.h>

#include <Raindrop/Exceptions/VulkanExceptions.hpp>

namespace Raindrop::Internal::Graphics::Queues{
	Manager::Manager(Context& context) :
			_context{context},
			_graphicsFamily{INVALID_FAMILY_INDEX},
			_presentFamily{INVALID_FAMILY_INDEX},
			_transfertFamily{INVALID_FAMILY_INDEX},
			_graphicsQueue{VK_NULL_HANDLE},
			_presentQueue{VK_NULL_HANDLE},
			_transfertQueue{VK_NULL_HANDLE}
		{

		_context.getLogger().info("Constructing vulkan queue manager ...");
		auto queueFamilies = context.getPhysicalDevice().queueFamilyProperties();

		findGraphicsFamily(queueFamilies);
		findPresentFamily(queueFamilies);
		findTransfertFamily(queueFamilies);
		getQueues(queueFamilies);
	}

	Manager::~Manager(){
		_context.getLogger().info("Destroying vulkan queue manager ...");
	}

	void Manager::findGraphicsFamily(std::vector<VkQueueFamilyProperties>& families){
		_context.getLogger().trace("Looking for graphics queue family ...");

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
		_context.getLogger().trace("Looking for present queue family ...");

		// Check is the graphics family also has present capabilities
		{
			VkBool32 supported = VK_FALSE;
			
			Exceptions::checkVkOperation<VkPhysicalDevice>(
				vkGetPhysicalDeviceSurfaceSupportKHR(_context.getPhysicalDevice().get(), _graphicsFamily, _context.getWindow().surface(), &supported),
				"Failed to get physical device surface support",
				Exceptions::VulkanOperationType::QUERYING
			);

			if (supported){
				_presentFamily = _graphicsFamily;
				return;
			} 
		}

		// If not, then try to get the first family with present support
		for (std::size_t i=0; i<families.size(); i++){
			VkBool32 presentSupport = VK_FALSE;

			Exceptions::checkVkOperation<VkPhysicalDevice>(
				vkGetPhysicalDeviceSurfaceSupportKHR(_context.getPhysicalDevice().get(), i, _context.getWindow().surface(), &presentSupport),
				"Failed to get queue family physical device surface support",
				Exceptions::VulkanOperationType::QUERYING
			);

			if (presentSupport == VK_TRUE){
				_presentFamily = i;
				return;
			}
		}

		throw std::runtime_error("Failed to find a queue with present support");
	}

	void Manager::findTransfertFamily(std::vector<VkQueueFamilyProperties>& families){
		_context.getLogger().trace("Looking for transfert queue family ...");
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
		auto device = _context.getDevice().get();

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