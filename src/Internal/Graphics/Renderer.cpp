#include <Raindrop_internal/Graphics/Renderer.hpp>
#include <Raindrop_internal/Graphics/Context.hpp>
#include <Raindrop_internal/Context.hpp>
#include <Raindrop_internal/Events/Manager.hpp>
#include <Raindrop/Exceptions/VulkanExceptions.hpp>

namespace Raindrop::Internal::Graphics{
	struct Renderer::EventCache{
		Events::EventID swapchainResized;
	};

	Renderer::Renderer(Internal::Context& internal) :
			_internal{internal},
			_context{nullptr},
			_eventCache{nullptr}{
			
		_internal.getLogger()->info("Constructing graphics renderer...");

		try{
			_context = new Context(internal);
		} catch (const std::bad_alloc& e){
			internal.getLogger()->error("Failed to initialize graphics Context : memory allocation failure | {}", e.what());
			throw;
		} catch (const std::exception& e){
			internal.getLogger()->error("Failed to initialize graphic Context : {}", e.what());
			throw;
		}

		createRenderCommandPool();
		allocateFrameCommandBuffers();

		_eventCache = new EventCache;

		registerEvents();
	}

	Renderer::~Renderer(){
		_internal.getLogger()->info("Destroying graphics renderer...");
		_context->getDevice().waitIdle();

		if (_eventCache != nullptr){
			unregisterEvents();
			
			delete _eventCache;
			_eventCache = nullptr;
		}
		
		freeFrameCommandBuffers();
		destroyRenderCommandPool();

		if (_context != nullptr){
			delete _context;
			_context = nullptr;
		}
	}

	void Renderer::registerEvents(){
		auto& eventManager = _context->getInternalContext().getEventManager();

		_eventCache->swapchainResized = eventManager.registerEvent("Swapchain.Resized");
	}

	void Renderer::unregisterEvents(){
		auto& eventManager = _context->getInternalContext().getEventManager();

		eventManager.unregisterEvent(_eventCache->swapchainResized);
	}


	void Renderer::render(){
		auto& swapchain = _context->getSwapchain();	
		auto& events = _context->getInternalContext().getEventManager();
		
		VkCommandBuffer commandBuffer = beginFrame();
		if (commandBuffer != nullptr){

			swapchain.beginRenderPass(commandBuffer);

			swapchain.endRenderPass(commandBuffer);

			endFrame();
		}
		
		_currentFrameID = (_currentFrameID + 1) % swapchain.frameCount();
	}

	void Renderer::events(){
		_context->getWindow().events();
	}
	
	void Renderer::createRenderCommandPool(){
		VkCommandPoolCreateInfo info{};
		info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		info.queueFamilyIndex = _context->getQueues().graphicsFamily();
		info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

		Exceptions::checkVkCreation<VkCommandPool>(
			vkCreateCommandPool(_context->getDevice().get(), &info, _context->getAllocationCallbacks(), &_renderCommandPool),
			"Failed to create frame command buffer pool"
		);
	}

	void Renderer::allocateFrameCommandBuffers(){
		std::size_t count = _context->getSwapchain().frameCount();
		_frameCommandBuffers.reserve(count);

		VkCommandBufferAllocateInfo info{};
		info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		info.commandPool = _renderCommandPool;
		info.commandBufferCount = static_cast<uint32_t>(count);
		info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		
		Exceptions::checkVkCreation<VkCommandBuffer>(
			vkAllocateCommandBuffers(_context->getDevice().get(), &info, _frameCommandBuffers.data()),
			"Failed to allocate frame command buffers"
		);
	}

	void Renderer::freeFrameCommandBuffers(){
		if (!_frameCommandBuffers.empty()){
			vkFreeCommandBuffers(_context->getDevice().get(), _renderCommandPool, static_cast<uint32_t>(_frameCommandBuffers.size()), _frameCommandBuffers.data());
			_frameCommandBuffers.clear();
		}
	}

	void Renderer::destroyRenderCommandPool(){
		if (_renderCommandPool != VK_NULL_HANDLE){
			vkDestroyCommandPool(_context->getDevice().get(), _renderCommandPool, _context->getAllocationCallbacks());
		}
	}

	VkCommandBuffer Renderer::beginFrame(){
		auto& window = _context->getWindow();
		auto& swapchain = _context->getSwapchain();

		VkResult result = swapchain.acquireNextImage();
		if (result == VK_ERROR_OUT_OF_DATE_KHR) {
			updateSwapchainSize();
			return nullptr;
		}

		if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
			throw std::runtime_error("failed to acquire swap chain image!");
		}

		auto commandBuffer = _frameCommandBuffers[_currentFrameID];
		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

		Exceptions::checkVkOperation<VkCommandBuffer>(
			vkBeginCommandBuffer(commandBuffer, &beginInfo),
			"Failed to begin command buffer",
			Exceptions::VulkanOperationType::BEGIN
		);

		return commandBuffer;
	}

	void Renderer::endFrame(){
		auto& window = _context->getWindow();
		auto& swapchain = _context->getSwapchain();

		VkCommandBuffer commandBuffer = _frameCommandBuffers[_currentFrameID];

		Exceptions::checkVkOperation<VkCommandBuffer>(
			vkEndCommandBuffer(commandBuffer),
			"Failed to end command buffer",
			Exceptions::VulkanOperationType::END
		);
		
		VkResult result = swapchain.submitCommandBuffer(&commandBuffer);

		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || window.resized()){
			updateSwapchainSize();
		} else if (result != VK_SUCCESS){
			throw Exceptions::VulkanResourceOperationException(result, "Failed to submit command buffer", Exceptions::VulkanOperationType::SUBMIT, VK_OBJECT_TYPE_COMMAND_BUFFER);
		}
	}

	void Renderer::updateSwapchainSize(){
		auto& window = _context->getWindow();
		auto& swapchain = _context->getSwapchain();
		
		auto size = window.getSize();
		window.resetResizedFlags();
		swapchain.setExtent(VkExtent2D{size.x, size.y});
		swapchain.rebuildSwapchain();
		
		{
			auto& eventManager = _context->getInternalContext().getEventManager();
			eventManager.trigger(_eventCache->swapchainResized, Maths::uvec2(size));
		}
	}

	Context& Renderer::getContext(){
		return *_context;
	}
}