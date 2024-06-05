#include <Raindrop_internal/Graphics/Queues.hpp>
#include <Raindrop_internal/Graphics/Context.hpp>
#include <Raindrop/Exceptions/VulkanExceptions.hpp>

namespace Raindrop::Internal::Graphics{
	Queue::Queue(Context& context, const VkQueue& queue) :
		_context{&context},
		_queue{queue}{}

	Queue::~Queue(){}

	VkQueue Queue::get() const{
		return _queue;
	}

	void Queue::waitIdle(){
		Exceptions::checkVkOperation<VkQueue>(
			vkQueueWaitIdle(_queue),
			"Failed to wait queue to be idle",
			Exceptions::VulkanOperationType::WAIT,
			_context->getLogger()
		);
	}

	Queues::Queues(Context& context) : _context{context}{

		auto& physicalDevice = _context.getPhysicalDevice();

		uint32_t count = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice.get(), &count, nullptr);

		if (count == 0){
			throw std::runtime_error("Cannot find a single queue family");
		}

		std::vector<VkQueueFamilyProperties> properties(static_cast<std::size_t>(count));
		vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice.get(), &count, properties.data());

		for (std::size_t i=0; i<properties.size(); i++){
			_families.push_back(QueueFamily(_context, static_cast<uint32_t>(i), properties[i]));
		}
	}

	Queues::~Queues(){
		_families.clear();
	}

	const std::vector<QueueFamily>& Queues::getFamilies() const{
		return _families;
	}


	std::list<std::pair<QueueFamily&, int>> Queues::findSuitable(const VkQueueFlags& requiredFlags, const VkSurfaceKHR& surface){
		std::list<std::pair<QueueFamily&, int>> out;

		const bool presentRequied = surface != VK_NULL_HANDLE;

		for (auto& family : _families){
			auto flags = family.getQueueFlags();

			if (surface != VK_NULL_HANDLE){
				if (family.isPresentSupported(surface) == false) continue;
			}

			int score = (flags ^ requiredFlags) * (2 * int((flags & requiredFlags) == requiredFlags) - 1);

			if (score > 0){
				out.push_back(std::make_pair(std::ref(family), score));
			}
		}

		struct{
			bool operator()(const std::pair<const QueueFamily&, int>& a, const std::pair<const QueueFamily&, int>& b) const {
				return a.second < b.second; 
			}
		} customLess;

		out.sort(customLess);

		return out;
	}

	//--------------------------------------------------------------------
	//-----------------          QUEUE FAMILY            -----------------
	//--------------------------------------------------------------------

	QueueFamily::QueueFamily(Context& context, const uint32_t& index, const VkQueueFamilyProperties& properties) : 
		_context{context},
		_index{index},
		_properties{properties}
	{

		auto& device = _context.getDevice();

		for (uint32_t i=0; i<properties.queueCount; i++){
			VkQueue queue = VK_NULL_HANDLE;
			vkGetDeviceQueue(device.get(), index, i, &queue);

			_queues.push_back(Queue(_context, queue));
		}
	}
	
	QueueFamily::~QueueFamily(){}

	uint32_t QueueFamily::getIndex() const{
		return _index;
	}

	uint32_t QueueFamily::getQueueCount() const{
		return _properties.queueCount;
	}

	VkQueueFlags QueueFamily::getQueueFlags() const{
		return _properties.queueFlags;
	}

	bool QueueFamily::isPresentSupported(VkSurfaceKHR surface) const{
		auto& physicalDevice = _context.getPhysicalDevice();
		
		VkBool32 supported = VK_FALSE;
		Exceptions::checkVkOperation<VkQueue>(
			vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice.get(), _index, surface, &supported),
			"Failed to get surface support",
			Exceptions::VulkanOperationType::QUERYING,
			_context.getLogger()
		);

		return static_cast<bool>(supported);
	}

	const std::vector<Queue>& QueueFamily::getQueues() const{
		return _queues;
	}

	const Queue& QueueFamily::getQueue(const std::size_t& id) const{
		return _queues.at(id);
	}

	Queue& QueueFamily::getQueue(const std::size_t& id){
		return _queues.at(id);
	}

}