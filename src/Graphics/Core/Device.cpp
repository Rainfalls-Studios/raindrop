#include <Raindrop/Graphics/Core/Device.hpp>
#include <Raindrop/Graphics/Context.hpp>

#include <spdlog/spdlog.h>

#include <Raindrop/Exceptions/VulkanExceptions.hpp>

namespace Raindrop::Graphics::Core{
	Device::Device() noexcept :
		_context{nullptr},
		_device{VK_NULL_HANDLE}
	{}

	Device::~Device(){
		release();
	}
	
	void Device::initialize(Context& context){
		_context = &context;
		_context->logger->info("Constructing vulkan device ...");


		auto& allocationCallbacks = _context->allocationCallbacks;
		auto& physicalDevice = _context->physicalDevice;

		VkDeviceCreateInfo info{};
		info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		
		const VkPhysicalDeviceFeatures& features = physicalDevice.getFeatures();
		info.pEnabledFeatures = &features;

		info.ppEnabledExtensionNames = _requiredExtensions.data();
		info.enabledExtensionCount = static_cast<uint32_t>(_requiredExtensions.size());

		info.ppEnabledLayerNames = _requiredLayers.data();
		info.enabledLayerCount = static_cast<uint32_t>(_requiredLayers.size());

		float queuePriority = 1.f;

		VkDeviceQueueCreateInfo queueCreateInfo{};
		queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfo.queueCount = 1;
		queueCreateInfo.pQueuePriorities = &queuePriority;
		queueCreateInfo.queueFamilyIndex = 0;

		info.pQueueCreateInfos = &queueCreateInfo;
		info.queueCreateInfoCount = 1;

		Exceptions::checkVkCreation<VkDevice>(
			vkCreateDevice(physicalDevice.get(), &info, allocationCallbacks, &_device),
			"Failed to create device",
			_context->logger
		);
		
		_context->logger->info("Vulkan device created !");
	}

	void Device::release(){
		if (_context && _device != VK_NULL_HANDLE){
			vkDestroyDevice(_device, _context->allocationCallbacks);
		}

		_device = VK_NULL_HANDLE;
		_context = nullptr;
	}

	uint32_t Device::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties){

		auto& physicalDevice = _context->physicalDevice;
		VkPhysicalDeviceMemoryProperties memProperties;
		vkGetPhysicalDeviceMemoryProperties(physicalDevice.get(), &memProperties);

		for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
			if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
				return i;
			}
		}

		_context->logger->warn("Failed to find suitable memory type filter (filter : {}; properties: {})", typeFilter, properties);
		throw std::runtime_error("failed to find suitable memory type");
	}

	void Device::waitIdle(){
		vkDeviceWaitIdle(_device);
	}

	const VkDevice& Device::get() const noexcept{
		return _device;
	}

	void Device::requireExtension(const char* extension){
		_requiredExtensions.push_back(extension);
	}

	void Device::requireLayer(const char* layer){
		_requiredLayers.push_back(layer);
	}


	static constexpr VkQueueFlags PRESENT_BIT = 1 << 31;

	struct QueueSearchData{
		const std::vector<VkQueueFlags>& candidates;
		const VkQueueFlags& requiredFlags;
		const VkQueueFlags& currentFlags;
		const std::size_t& index;
		std::vector<std::size_t>& currentSolution;
		std::vector<std::size_t>& bestSolution;
	};

	void findBestQueues(QueueSearchData data) {
		const std::vector<VkQueueFlags>& candidates = data.candidates;
		const VkQueueFlags& requiredFlags = data.requiredFlags;
		const VkQueueFlags& currentFlags = data.currentFlags;
		const std::size_t& index = data.index;
		std::vector<std::size_t>& currentSolution = data.currentSolution;
		std::vector<std::size_t>& bestSolution = data.bestSolution;


		// If the current solutions cover all the needs
		if ((currentFlags & requiredFlags) == requiredFlags) {

			// If ths collection is smaller than the already present one
			if (currentSolution.size() < bestSolution.size()) {

				// We have found a new best solution
				bestSolution = currentSolution;
			}
			return;
		}
		
		// If we are at the end of the candidats vector. we can't check further
		if (index == candidates.size() || currentSolution.size() >= bestSolution.size()) {
			return;
		}

		// Include the current candidate
		currentSolution.push_back(candidates[index]);
		findBestQueues({candidates, requiredFlags, currentFlags | candidates[index], index + 1, currentSolution, bestSolution});
		
		// Exclude the current candidate
		currentSolution.pop_back();
		findBestQueues({candidates, requiredFlags, currentFlags, index + 1, currentSolution, bestSolution});
	}

	std::vector<std::size_t> getBestQueues(const std::vector<VkQueueFlags>& candidates, const VkQueueFlags& requiredFlags) {
		std::vector<std::size_t> bestSolution; // Initially set to an impossibly large solution size
		std::vector<std::size_t> currentSolution;

		findBestQueues({candidates, requiredFlags, 0, 0, currentSolution, bestSolution});

		return bestSolution;
	}

	void Device::findQueues()
	const bool isSurfaceSupportRequired = _surface != VK_NULL_HANDLE;

		VkQueueFlags requiredFlags = VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_TRANSFER_BIT;
		if (isSurfaceSupportRequired){
			requiredFlags |= PRESENT_BIT;
		}

		std::vector<VkQueueFlags> candidats(_families.size());


		for (std::size_t i=0; i<_families.size(); i++){
			const QueueFamily& family = _families[i];
			VkQueueFlags& candidat = candidats[i];

			candidat = family.getFlags();

			if (isSurfaceSupportRequired && family.isPresentSupported(_surface)){
				candidat |= PRESENT_BIT;
			}
		}

		std::vector<std::size_t> bestCombination = getBestQueues(candidats, requiredFlags);

		for (auto& id : bestCombination){
			spdlog::info("{}", id);
		}
}