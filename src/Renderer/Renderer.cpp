#include <Raindrop/Renderer/Renderer.hpp>
#include <Raindrop/Renderer/Context.hpp>
#include <spdlog/spdlog.h>

// TEST
#include <Raindrop/Renderer/Pipelines/Triangle.hpp>
static std::unique_ptr<Raindrop::Renderer::Pipelines::Triangle> shader;

namespace Raindrop::Renderer{
	Renderer::Renderer(::Raindrop::Context& context) : 
			_context{nullptr},
			_renderCommandPool{VK_NULL_HANDLE},
			_currentFrameID{0}{
		spdlog::info("Constructing renderer ...");
		_context = new Context(context);

		createRenderCommandPool();
		allocateFrameCommandBuffers();

		shader = std::make_unique<Pipelines::Triangle>(*_context);
	}

	Renderer::~Renderer(){
		spdlog::info("Destructing renderer ...");
		_context->device.waitIdle();

		shader = nullptr;

		freeFrameCommandBuffers();
		destroyRenderCommandPool();

		delete _context;
	}

	void Renderer::render(){
		auto& swapchain = _context->swapchain;
		
		VkCommandBuffer commandBuffer = beginFrame();
		if (commandBuffer != nullptr){
			

			_context->scene.beginRenderPass(commandBuffer);
			shader->bind(commandBuffer);
			vkCmdDraw(commandBuffer, 3, 1, 0, 0);
			_context->scene.endRenderPass(commandBuffer);

			swapchain.beginRenderPass(commandBuffer);
			_context->scene.swapchainRender(commandBuffer);
			swapchain.endRenderPass(commandBuffer);

			endFrame();
		}
		
		_currentFrameID = (_currentFrameID + 1) % _context->swapchain.frameCount();
	}

	void Renderer::events(){
		_context->window.events();
	}
	
	void Renderer::createRenderCommandPool(){
		VkCommandPoolCreateInfo info{};
		info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		info.queueFamilyIndex = _context->queues.graphicsFamily();
		info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

		if (vkCreateCommandPool(_context->device.get(), &info, _context->allocationCallbacks, &_renderCommandPool) != VK_SUCCESS){
			throw std::runtime_error("Failed to create render command pool");
		}
	}

	void Renderer::allocateFrameCommandBuffers(){
		std::size_t count = _context->swapchain.frameCount();
		_frameCommandBuffers.reserve(count);

		VkCommandBufferAllocateInfo info{};
		info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		info.commandPool = _renderCommandPool;
		info.commandBufferCount = static_cast<uint32_t>(count);
		info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		
		if (vkAllocateCommandBuffers(_context->device.get(), &info, _frameCommandBuffers.data()) != VK_SUCCESS){
			throw std::runtime_error("Failed to allocate frame command buffers");
		}
	}

	void Renderer::freeFrameCommandBuffers(){
		if (!_frameCommandBuffers.empty()){
			vkFreeCommandBuffers(_context->device.get(), _renderCommandPool, static_cast<uint32_t>(_frameCommandBuffers.size()), _frameCommandBuffers.data());
			_frameCommandBuffers.clear();
		}
	}

	void Renderer::destroyRenderCommandPool(){
		if (_renderCommandPool != VK_NULL_HANDLE){
			vkDestroyCommandPool(_context->device.get(), _renderCommandPool, _context->allocationCallbacks);
		}
	}

	VkCommandBuffer Renderer::beginFrame(){
		auto& window = _context->window;
		auto& swapchain = _context->swapchain;

		VkResult result = swapchain.acquireNextImage();
		if (result == VK_ERROR_OUT_OF_DATE_KHR) {
			auto size = window.getSize();
			swapchain.setExtent(VkExtent2D{size.x, size.y});
			swapchain.rebuildSwapchain();
			_context->scene.resize(size);
			return nullptr;
		}

		if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
			throw std::runtime_error("failed to acquire swap chain image!");
		}

		auto commandBuffer = _frameCommandBuffers[_currentFrameID];
		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

		if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
			throw std::runtime_error("failed to begin recording command buffer!");
		}
		return commandBuffer;
	}

	void Renderer::endFrame(){
		auto& window = _context->window;
		auto& swapchain = _context->swapchain;

		VkCommandBuffer commandBuffer = _frameCommandBuffers[_currentFrameID];

		if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS)
			throw std::runtime_error("failed to record command buffer");
		
		VkResult result = swapchain.submitCommandBuffer(&commandBuffer);

		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || window.resized()){
			auto size = window.getSize();
			window.resetResizedFlags();
			swapchain.setExtent(VkExtent2D{size.x, size.y});
			swapchain.rebuildSwapchain();
			_context->scene.resize(size);
		} else if (result != VK_SUCCESS){
			throw std::runtime_error("failed to submit the command buffer");
		}
	}
}