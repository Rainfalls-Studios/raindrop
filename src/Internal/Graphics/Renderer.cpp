#include <Raindrop_internal/Graphics/Renderer.hpp>
#include <Raindrop_internal/Graphics/Context.hpp>
#include <Raindrop_internal/Context.hpp>
#include <Raindrop_internal/Events/Manager.hpp>
#include <Raindrop/Exceptions/VulkanExceptions.hpp>
#include <Raindrop_internal/Assets/Manager.hpp>
#include <Raindrop_internal/Graphics/ShaderLoader.hpp>

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

		createFrameCommandPool();
		allocateFrameCommandBuffers();

		_eventCache = new EventCache;
		registerEvents();

		_shaderLoader = std::make_shared<ShaderLoader>(*_context);
		_context->getInternalContext().getAssetManager().registerLoader("Shader", _shaderLoader);

		// _context->getSwapchain().setPresentMode(VK_PRESENT_MODE_FIFO_RELAXED_KHR);
		// _context->getSwapchain().rebuildSwapchain();
	}

	Renderer::~Renderer(){
		_internal.getLogger()->info("Destroying graphics renderer...");
		_context->getDevice().waitIdle();

		_shaderLoader.reset();

		if (_eventCache != nullptr){
			unregisterEvents();
			
			delete _eventCache;
			_eventCache = nullptr;
		}
		
		freeFrameCommandBuffers();
		destroyFrameCommandPool();

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
	
	void Renderer::createFrameCommandPool(){
		CommandPoolConfigInfo info;

		auto families = _context->getQueues().findSuitable(VK_QUEUE_GRAPHICS_BIT, _context->getWindow().surface());

		if (families.empty()){
			_context->getLogger()->error("Couldd't find a suitable queue family that has both graphics and present capabilities");
			throw std::runtime_error("Counld not find suitable queue families for frame command buffers");
		}

		QueueFamily& family = families.front().first;

		// Since the command buffers are reset every frame
		info.flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT | VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
		info.familyIndex = family.getIndex();

		_context->frame.getCommandPool() = new CommandPool(*_context, info);

		_context->frame.getQueue() = &family.getQueue(0);
	}

	void Renderer::allocateFrameCommandBuffers(){
		auto& pool = _context->frame.getCommandPool();
		if (!pool){
			_context->getLogger()->error("Cannot create frame command buffers without a valid command pool ! FrameCommandPool is nullptr");
			throw std::runtime_error("The pool is invalid");
		}

		std::size_t count = _context->getSwapchain().frameCount();
		_frameCommandBuffers = pool->allocate(count, VK_COMMAND_BUFFER_LEVEL_PRIMARY);
	}

	void Renderer::freeFrameCommandBuffers(){
		auto& pool = _context->frame.getCommandPool();
		if (!pool){
			_context->getLogger()->error("Cannot free frame command buffers without a valid command pool ! FrameCommandPool is nullptr");
			throw std::runtime_error("The pool is invalid");
		}

		pool->free(_frameCommandBuffers);
	}

	void Renderer::destroyFrameCommandPool(){
		auto& pool = _context->frame.getCommandPool();
		if (pool){
			delete pool;
			pool = nullptr;
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