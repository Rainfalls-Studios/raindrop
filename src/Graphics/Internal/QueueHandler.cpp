#include <Raindrop/Graphics/Internal/QueueHandler.hpp>
#include <Raindrop/Graphics/Internal/Context.hpp>
#include <Raindrop/Graphics/Internal/QueueFamily.hpp>

namespace Raindrop::Graphics::Internal{
	QueueHandler::QueueHandler(Context& context) : _context{context}{
		_context.logger().info("Initializing queue handler...");

		populateFamilies();

		_context.logger().info("Queue handler initialized");
	}

	QueueHandler::~QueueHandler(){
		_context.logger().info("Terminating queue handler...");

		_context.logger().info("Queue handler terminated");
	}

	void QueueHandler::populateFamilies(){
		auto physicalDevice = _context.physicalDevice().get();
		auto surface = _context.window().surface();

		uint32_t count = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &count, nullptr);

		std::vector<VkQueueFamilyProperties> properties(count);
		vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &count, properties.data());

		for (std::size_t i=0; i<properties.size(); i++){
			auto& queueProperties = properties[i];

			VkBool32 supportPresent;
			if (vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, i, surface, &supportPresent) != VK_SUCCESS){
				_context.logger().error("Failed to query family surface support");
				throw std::runtime_error("Failed to query family surface support");
			}

			QueueProperties queueInfo{
				queueProperties.queueFlags,
				static_cast<bool>(supportPresent)
			};
			
			_families.push_back(std::make_unique<QueueFamily>(*this, i, queueProperties.queueCount, queueInfo));
		}
	}

	QueueFamily& QueueHandler::get(std::size_t index){
		auto it = std::find_if(
			_families.begin(),
			_families.end(),
			[&index](const std::unique_ptr<QueueFamily>& queueFamily){
				return queueFamily->index() == index;
			}
		);

		if (it == _families.end()){
			_context.logger().warn("Failed to find queue family at %d index", index);
			throw std::runtime_error("Failed to find queue family at given index");
		}

		return *it->get();
	}

	const QueueFamily& QueueHandler::get(std::size_t index) const{
		auto it = std::find_if(
			_families.begin(),
			_families.end(),
			[&index](const std::unique_ptr<QueueFamily>& queueFamily){
				return queueFamily->index() == index;
			}
		);

		if (it == _families.end()){
			_context.logger().warn("Failed to find queue family at %d index", index);
			throw std::runtime_error("Failed to find queue family at given index");
		}

		return *it->get();
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

	std::list<std::reference_wrapper<QueueFamily>> QueueHandler::getByProperies(const QueueProperties& properties){
		auto& device = _context.device();

		std::vector<int> suitabilities(_families.size());

		for (auto& family : _families){
			auto& suitability = suitabilities[family->index()];
			const auto& familyProperties = family->properties();

			suitability = -1;

			if ((properties.flags & familyProperties.flags) != familyProperties.flags) continue;
			if (properties.present && !familyProperties.present) continue;

			suitability = static_cast<int>((sizeof(VkQueueFlags) * 8) - countSetBitsDifference(familyProperties.flags, properties.flags));
		}

		std::list<std::reference_wrapper<QueueFamily>> output;
		for (auto& family : _families){
			output.push_back(*family);
		}

		output.remove_if(
			[&suitabilities](const QueueFamily& family){
				return suitabilities[family.index()] < 0;
			}
		);

		output.sort([&suitabilities](const QueueFamily& a, const QueueFamily& b){
			return suitabilities[a.index()] < suitabilities[b.index()];
		});

		return output;
	}
}