#include <Raindrop/Graphics/Sync/Queue.hpp>
#include <Raindrop/Graphics/Core/Context.hpp>
#include <Raindrop/Graphics/Sync/Context.hpp>

namespace Raindrop::Graphics::Sync{
	Queue::Queue() : 
		_context{nullptr},
		_core{nullptr},
		_family{nullptr},
		_queue{VK_NULL_HANDLE},
		_index{INVALID_INDEX}
	{}

	Queue::~Queue(){
		release();
	}

	void Queue::initialize(Context& context, Core::Context& core, QueueFamily& family, const std::size_t& queueIndex){
		_context = &context;
		_core = &core;
		_family = &family;

		_context->logger->info("{} : {}", static_cast<uint32_t>(family.getIndex()), static_cast<uint32_t>(queueIndex));

		vkGetDeviceQueue(
			_core->device.get(),
			static_cast<uint32_t>(_family->getIndex()),
			static_cast<uint32_t>(_index),
			&_queue
		);
	}

	void Queue::release(){
		_context = nullptr;
		_core = nullptr;
		_family = nullptr;
		_queue = VK_NULL_HANDLE;
		_index = INVALID_INDEX;
	}

	const VkQueue& Queue::get() const noexcept{
		return _queue;
	}

	const QueueFamily& Queue::getFamily() const noexcept{
		return *_family;
	}

	const std::size_t& Queue::getIndex() const noexcept{
		return _index;
	}
}