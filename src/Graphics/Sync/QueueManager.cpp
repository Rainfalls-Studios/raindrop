#include <Raindrop/Graphics/Sync/QueueManager.hpp>
#include <Raindrop/Graphics/Sync/Context.hpp>
#include <Raindrop/Graphics/Core/Context.hpp>

namespace Raindrop::Graphics::Sync{
	QueueManager::QueueManager() : 
		_context{nullptr},
		_core{nullptr},
		_surface{VK_NULL_HANDLE},
		_graphicsQueue{nullptr},
		_presentQueue{nullptr},
		_transfertQueue{nullptr}
	{}

	QueueManager::~QueueManager(){
		release();
	}

	void QueueManager::initialize(Context& context, Core::Context& core){
		_context = &context;
		_core = &core;

		std::size_t familyCount = _core->physicalDevice.getQueueProperties().size();
		_families.resize(familyCount);

		for (std::size_t i=0; i<familyCount; i++){
			_families[i].initialize(context, core, i);
		}


		findQueues();
	}

	void QueueManager::release(){
		_context = nullptr;
		_core = nullptr;
		_surface = VK_NULL_HANDLE;
		_graphicsQueue = nullptr;
		_presentQueue = nullptr;
		_transfertQueue = nullptr;
	}

	

	void QueueManager::findQueues(){
		
	}

	void QueueManager::requireSurfaceSupport(const VkSurfaceKHR& surface){
		_surface = surface;
	}

}