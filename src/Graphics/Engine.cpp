#include <Raindrop/Graphics/Engine.hpp>
#include <Raindrop/Graphics/Context.hpp>
#include <Raindrop/Graphics/Internal/Context.hpp>
#include <Raindrop/Graphics/Internal/CommandPool.hpp>
#include <Raindrop/Core/Engine.hpp>

namespace Raindrop::Graphics{
	Engine::Engine(Core::Engine& core) : 
			_currentFrameID{0}{
		_context = std::make_unique<Context>(core);
		_context->logger().info("Initializing Graphics Engine...");

		allocateCommandBuffers();

		_context->logger().info("Graphics Engine initialized without any critical error");
	}

	Engine::~Engine(){
		_context->logger().info("Terminating Graphics Engine...");

		_context->internal().device().waitIdle();

		freeCommandBuffers();
		_context->logger().info("Graphics Engine terminated without any critical error");

		_context.reset();
	}

	void Engine::render(){
		_context->internal().window().events();
		
		VkCommandBuffer commandBuffer = beginFrame();

		if (commandBuffer != nullptr){
			auto& swapchain = _context->swapchain();
			_context->core().registry().set("Renderer.commandBuffer", commandBuffer);

			_context->core().eventManager().trigger("Renderer.PreSwapchainRenderPass");

			swapchain.beginRenderPass(commandBuffer);
			_context->core().eventManager().trigger("Renderer.SwapchainRenderPass");
			swapchain.endRenderPass(commandBuffer);

			_context->core().eventManager().trigger("Renderer.PostSwapchainRenderPass");

			endFrame();
		}

		_currentFrameID = (_currentFrameID + 1) % _context->swapchain().frameCount();
	}

	void Engine::allocateCommandBuffers(){
		auto& swapchain = _context->swapchain();
		_framesCommandBuffers.resize(swapchain.frameCount());

		_framesCommandPool = &_context->internal().commandPools().pool({VK_QUEUE_GRAPHICS_BIT, true}, VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT);
		
		if (_framesCommandPool->allocate(_framesCommandBuffers.data(), VK_COMMAND_BUFFER_LEVEL_PRIMARY, _framesCommandBuffers.size()) != VK_SUCCESS){
			throw std::runtime_error("Failed to create frames graphics command buffers");
		}
	}

	void Engine::freeCommandBuffers(){
		auto& device = _context->internal().device();
		_framesCommandPool->free(_framesCommandBuffers.data(), _framesCommandBuffers.size());
	}
	
	VkCommandBuffer Engine::beginFrame(){
		auto& window = _context->internal().window();
		auto& swapchain = _context->swapchain();

		VkResult result = swapchain.acquireNextImage();
		if (result == VK_ERROR_OUT_OF_DATE_KHR) {
			auto size = window.getSize();
			swapchain.setExtent(VkExtent2D{size.x, size.y});
			swapchain.rebuildSwapchain();
			return nullptr;
		}

		if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
			throw std::runtime_error("failed to acquire swap chain image!");
		}

		auto commandBuffer = _framesCommandBuffers[_currentFrameID];
		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

		if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
			throw std::runtime_error("failed to begin recording command buffer!");
		}
		return commandBuffer;
	}

	void Engine::endFrame(){
		auto& window = _context->internal().window();
		auto& swapchain = _context->swapchain();

		VkCommandBuffer commandBuffer = _framesCommandBuffers[_currentFrameID];

		if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS)
			throw std::runtime_error("failed to record command buffer");
		
		VkResult result = swapchain.submitCommandBuffer(&commandBuffer);

		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || window.resized()){
			auto size = window.getSize();
			window.resetResizedFlag();
			swapchain.setExtent(VkExtent2D{size.x, size.y});
			swapchain.rebuildSwapchain();
		} else if (result != VK_SUCCESS){
			throw std::runtime_error("failed to submit the command buffer");
		}
	}

	void Engine::loadFromFile(const std::filesystem::path& path){
		_context->loader().loadFile(path);
	}
	
	const Context& Engine::context() const{
		return *_context;
	}

	Context& Engine::context(){
		return *_context;
	}

	VkCommandBuffer Engine::currentFramebuffer() const{
		return _framesCommandBuffers[_currentFrameID];
	}

}