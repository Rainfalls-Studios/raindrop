#include <Raindrop/Graphics/Internal/Queues.hpp>
#include <Raindrop/Graphics/Internal/Context.hpp>
#include <Raindrop/Core/Engine.hpp>

namespace Raindrop::Graphics::Internal{
	Queues::Queues(Context& context) : _context{context}{
		_graphicsFamily = nullptr;
		_presentFamily = nullptr;

		getFamilies();
	}

	void Queues::getFamilies(){
		auto list = _context.queueHandler().getByProperies({VK_QUEUE_GRAPHICS_BIT, true});
		if (!list.empty()){
			auto& family = list.front();
			_graphicsFamily = &family.get();
			_presentFamily = _graphicsFamily;
		} else {
			list = _context.queueHandler().getByProperies({VK_QUEUE_GRAPHICS_BIT, false});
			if (!list.empty()){
				auto& family = list.front();
				_graphicsFamily = &family.get();
			}

			list = _context.queueHandler().getByProperies({0, true});
			if (!list.empty()){
				auto& family = list.front();
				_presentFamily = &family.get();
			}
		}

		auto& registry = _context.graphics().core().registry();
		registry.set("queues.graphicsFamily", _graphicsFamily ? _graphicsFamily->index() : 0);
		registry.set("queues.presentFamily", _presentFamily ? _presentFamily->index() : 0);
	}

	Queues::~Queues(){}
	QueueFamily& Queues::graphicsFamily(){
		if (_graphicsFamily) return *_graphicsFamily;
		throw std::runtime_error("family not available");
	}

	QueueFamily& Queues::presentFamily(){
		if (_presentFamily) return *_presentFamily;
		throw std::runtime_error("family not available");
	}
}