#include <Raindrop/Graphics/Sync/QueueFamily.hpp>
#include <Raindrop/Graphics/Sync/Context.hpp>
#include <Raindrop/Graphics/Core/Context.hpp>
#include <Raindrop/Exceptions/VulkanExceptions.hpp>

namespace Raindrop::Graphics::Sync{
	QueueFamily::QueueFamily() : 
		_context{nullptr},
		_index{INVALID_INDEX},
		_queueCount{0},
		_flags{0},
		_queues{}
	{}

	QueueFamily::~QueueFamily(){
		release();
	}

	void QueueFamily::initialize(Context& context, Core::Context& core, const std::size_t& familyIndex){
		_context = &context;
		_core = &core;

		const VkQueueFamilyProperties& properties = _core->physicalDevice.getQueueProperties().at(familyIndex);

		_index = familyIndex;
		_queueCount = static_cast<std::size_t>(properties.queueCount);
		_flags = properties.queueFlags;

		_queues.resize(_queueCount);
		
		for (std::size_t i=0; i<_queueCount; i++){
			_queues[i].initialize(context, core, *this, i);
		}
	}

	bool QueueFamily::isPresentSupported(const VkSurfaceKHR& surface) const{
		
		VkBool32 supported = VK_FALSE;
		Exceptions::checkVkOperation<VkQueue>(
			vkGetPhysicalDeviceSurfaceSupportKHR(_core->physicalDevice.get(), _index, surface, &supported),
			"Failed to get surface support",
			Exceptions::VulkanOperationType::QUERYING,
			_context->logger
		);

		return supported == VK_TRUE;
	}


	void QueueFamily::release(){
		_context = nullptr;
		_index = INVALID_INDEX;
		_queueCount = 0;
		_flags = 0;
		_queues.clear();
	}

	const std::size_t& QueueFamily::getIndex() const noexcept{
		return _index;
	}

	const std::size_t& QueueFamily::getQueueCount() const noexcept{
		return _queueCount;
	}

	const VkQueueFlags& QueueFamily::getFlags() const noexcept{
		return _flags;
	}

	const std::vector<Queue>& QueueFamily::getQueues() const noexcept{
		return _queues;
	}

	const Queue& QueueFamily::getQueue(const std::size_t& index) const{
		return _queues.at(index);
	}
}	