#include <Raindrop/Graphics/Internal/structs.hpp>

namespace Raindrop::Graphics::Internal{
	QueueProperties::QueueProperties(VkQueueFlags flags, bool present) : flags{flags}, present{present}{}
	QueueProperties& QueueProperties::operator=(VkQueueFlags flags){
		this->flags = flags;
		present = false;
		return *this;
	}

	bool QueueProperties::operator==(const QueueProperties& other) const{
		return present == other.present && flags == other.flags;
	}

	bool QueueProperties::operator!=(const QueueProperties& other) const{
		return present != other.present || flags != other.flags;
	}

	std::size_t QueueProperties::hash(const QueueProperties& properties){
		return std::hash<bool>{}(properties.present) ^ std::hash<uint32_t>{}(static_cast<uint32_t>(properties.flags));
	}
}