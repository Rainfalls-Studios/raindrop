#include <Raindrop/Graphics/Internal/QueueFamily.hpp>
#include <Raindrop/Graphics/Internal/Queue.hpp>

namespace Raindrop::Graphics::Internal{
	QueueFamily::QueueFamily(QueueHandler& queueHandler, std::size_t familyIndex, std::size_t queueCount, const QueueProperties& properties) : 
		_queueHandler{queueHandler},
		_familyIndex{familyIndex},
		_properties{properties}{}

	QueueFamily::~QueueFamily(){
	}

	VkQueueFlags QueueFamily::flags() const{
		return _properties.flags;
	}
	
	const QueueProperties& QueueFamily::properties() const{
		return _properties;
	}

	std::size_t QueueFamily::index() const{
		return _familyIndex;
	}

	bool QueueFamily::supportPresent() const{
		return _properties.present;
	}

	Queue& QueueFamily::get(std::size_t index){
		return *_queues[index];
	}
	
	const Queue& QueueFamily::get(std::size_t index) const{
		return *_queues[index];
	}
}