#include <Raindrop/Graphics/Internal/QueueHandler.hpp>
#include <Raindrop/Graphics/Internal/Context.hpp>

namespace Raindrop::Graphics::Internal{
	struct QueueHandler::FamilyInfo{
		const VkQueueFamilyProperties& properties;
		uint32_t familyIndex;
		uint32_t suitablility;

		FamilyInfo(const VkQueueFamilyProperties& properties, uint32_t familyIndex) : properties{properties}, familyIndex{familyIndex}{}

		bool operator>(const FamilyInfo& other) const{
			return suitablility > other.suitablility;
		}

		bool operator<(const FamilyInfo& other) const{
			return suitablility > other.suitablility;
		}
	};

	QueueHandler::QueueHandler(Context& context) : _context{context}, _queues{10, QueueProperties::hash}{
		_context.logger.info("Initializing queue handler...");

		std::vector<VkQueueFamilyProperties> familyProperties = _context.device.getQueueFamilyProperties();
		_remainingQueueCount.reserve(familyProperties.size());

		for (uint32_t i=0; i<familyProperties.size(); i++){
			_remainingQueueCount[i] = familyProperties[i].queueCount;
		}

		_context.logger.info("Queue handler initialized");
	}

	QueueHandler::~QueueHandler(){
		_context.logger.info("Terminating queue handler...");

		_context.logger.info("Queue handler terminated");
	}

	VkQueue QueueHandler::get(const QueueProperties& properties){
		auto it = _queues.find(properties);
		if (it != _queues.end()) return it->second.queue;
	}

	VkQueue QueueHandler::query(const QueueProperties& properties){
		auto& device = _context.device;

		std::vector<VkQueueFamilyProperties> familyProperties = _context.device.getQueueFamilyProperties();
		std::list<FamilyInfo> infos;

		for (uint32_t i=0; i<familyProperties.size(); i++){
			infos.push_back(FamilyInfo(familyProperties[i], i));
		}

		infos.sort(std::greater<uint32_t>());

		FamilyInfo* bestCase = nullptr;
		uint32_t bestSuitability = -1;

		for (auto& info : infos){
			if (info.suitablility < 0) continue;
			if (info.suitablility > bestSuitability){
				bestCase = &info;
				bestSuitability = info.suitablility;
			}
		}

		if (bestCase == nullptr){
			_context.logger.warn("Could not find a unused suitable queue");
			throw std::runtime_error("Cannot find a unused suitable queue");
		}		

		QueueData queue;
		queue.familyIndex = bestCase->familyIndex;
		queue.queueIndex = _remainingQueueCount[queue.familyIndex]--;

		vkGetDeviceQueue(device.get(), queue.familyIndex, queue.queueIndex, &queue.queue);
		_queues[properties] = queue;
		return queue.queue;
	}

	static uint32_t countSetBitsDifference(uint32_t a, uint32_t b) {
		uint32_t xorResult = a ^ b;
		uint32_t count = 0;

		while (xorResult > 0) {
			count += xorResult & 1;
			xorResult >>= 1;
		}

		return count;
	}

	void QueueHandler::testFamiliesSuitability(const QueueProperties& properties, std::list<FamilyInfo>& families){
		auto& device = _context.device;

		for (auto& family : families){
			auto& familyProperties = family.properties;
			family.suitablility = -1;

			if (_remainingQueueCount[family.familyIndex] == 0) continue;
			if (!(properties.flags & familyProperties.queueFlags)) continue;

			if (properties.present){
				VkBool32 supported;
				vkGetPhysicalDeviceSurfaceSupportKHR(
					device.getPhysicalDevice().get(),
					family.familyIndex,
					_context.window.surface(),
					&supported
				);

				if (!supported) continue;
			}

			family.suitablility = (sizeof(VkQueueFlags) * 8) - countSetBitsDifference(familyProperties.queueFlags, properties.flags);
		}
	}

	QueueHandler::QueueProperties::QueueProperties(VkQueueFlags flags, bool present) : flags{flags}, present{present}{}
	QueueHandler::QueueProperties& QueueHandler::QueueProperties::operator=(VkQueueFlags flags){
		this->flags = flags;
		present = false;
		return *this;
	}

	bool QueueHandler::QueueProperties::operator==(const QueueProperties& other) const{
		return present == other.present && flags == other.flags;
	}

	bool QueueHandler::QueueProperties::operator!=(const QueueProperties& other) const{
		return present != other.present || flags != other.flags;
	}

	std::size_t QueueHandler::QueueProperties::hash(const QueueProperties& properties){
		return std::hash<bool>{}(properties.present) ^ std::hash<uint32_t>{}(static_cast<uint32_t>(properties.flags));
	}
}