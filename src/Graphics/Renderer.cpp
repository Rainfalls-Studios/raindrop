#include <Raindrop/Graphics/Renderer.hpp>
#include <Raindrop/Graphics/Context.hpp>
#include <spdlog/spdlog.h>
#include <Raindrop/Context.hpp>

#include <Raindrop/Graphics/Textures/Loader.hpp>
#include <Raindrop/Graphics/Models/Loader.hpp>
#include <Raindrop/Graphics/Models/Model.hpp>
#include <Raindrop/Graphics/Models/Mesh.hpp>
#include <Raindrop/Graphics/RenderSystems/RenderSystem.hpp>

#include <Raindrop/Components/Transformation.hpp>
#include <Raindrop/Components/Model.hpp>

#include <Raindrop/Exceptions/VulkanExceptions.hpp>

namespace Raindrop::Graphics{
	Renderer::Renderer(::Raindrop::Context& context) : 
			_context{nullptr},
			_renderCommandPool{VK_NULL_HANDLE},
			_currentFrameID{0}
		{
		
		spdlog::info("Constructing renderer ...");
		_context = new Context(context);

		createRenderCommandPool();
		allocateFrameCommandBuffers();

		_context->core.assetManager.createLoader<Textures::Loader>("Texture", *_context);
		_context->core.assetManager.createLoader<Models::Loader>("Model", *_context);

		_context->core.eventManager.registerEvent<VkCommandBuffer>("Renderer.baseFramebuffer.renderPass");
		_context->core.eventManager.registerEvent<VkCommandBuffer>("Renderer.frame");
		_context->core.eventManager.registerEvent<Maths::uvec2>("Renderer.swapchain.resized");
		
		_context->core.eventManager.trigger("Renderer.swapchain.resized", Maths::uvec2(_context->window.getSize()));
	}

	Renderer::~Renderer(){
		spdlog::info("Destroying renderer ...");
		_context->device.waitIdle();

		_context->core.assetManager.unregisterType("Model");
		_context->core.assetManager.unregisterType("Texture");

		freeFrameCommandBuffers();
		destroyRenderCommandPool();

		delete _context;
	}

	void Renderer::render(){
		auto& swapchain = _context->swapchain;	
		auto& events = _context->core.eventManager;
		
		VkCommandBuffer commandBuffer = beginFrame();
		if (commandBuffer != nullptr){
			
			events.trigger("Renderer.frame", commandBuffer);
			
			_context->baseFramebuffer.beginRenderPass(commandBuffer);
			events.trigger("Renderer.baseFramebuffer.renderPass", commandBuffer);
			_context->baseFramebuffer.endRenderPass(commandBuffer);

			swapchain.beginRenderPass(commandBuffer);
			_context->baseFramebuffer.swapchainRender(commandBuffer);
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

		Exceptions::checkVkCreation<VkCommandPool>(
			vkCreateCommandPool(_context->device.get(), &info, _context->allocationCallbacks, &_renderCommandPool),
			"Failed to create frame command buffer pool"
		);
	}

	void Renderer::allocateFrameCommandBuffers(){
		std::size_t count = _context->swapchain.frameCount();
		_frameCommandBuffers.reserve(count);

		VkCommandBufferAllocateInfo info{};
		info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		info.commandPool = _renderCommandPool;
		info.commandBufferCount = static_cast<uint32_t>(count);
		info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		
		Exceptions::checkVkCreation<VkCommandBuffer>(
			vkAllocateCommandBuffers(_context->device.get(), &info, _frameCommandBuffers.data()),
			"Failed to allocate frame command buffers"
		);
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
		auto& window = _context->window;
		auto& swapchain = _context->swapchain;

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
		auto& window = _context->window;
		auto& swapchain = _context->swapchain;
		
		auto size = window.getSize();
		window.resetResizedFlags();
		swapchain.setExtent(VkExtent2D{size.x, size.y});
		swapchain.rebuildSwapchain();
		_context->baseFramebuffer.resize(size);
		_context->core.eventManager.trigger("Renderer.swapchain.resized", Maths::uvec2(size));
	}

	Context& Renderer::context(){
		assert(_context != nullptr && "The context has not been initialiezed");
		return *_context;
	}

	MaterialWrapper Renderer::createMaterial(){
		auto material = Material(*_context);
		return registerMaterial(material);
	}
	
	Materials::Manager& Renderer::materials(){
		return _context->materials;
	}

	const Materials::Manager& Renderer::materials() const{
		return _context->materials;
	}
	
	MaterialWrapper Renderer::registerMaterial(const Material& material){
		return MaterialWrapper(*_context, _context->materials.registerMaterial(material));
	}

	void Renderer::destroyMaterial(const MaterialWrapper& wrapper){
		_context->materials.destroyMaterial(wrapper.ID());
	}

	RenderSystemWrapper<> Renderer::registerRenderSystem(std::unique_ptr<RenderSystem>&& system){
		auto ID = _context->renderSystems.create(std::move(system));
		return RenderSystemWrapper<>(*_context, ID);
	}

	void Renderer::destroyRenderSystem(const RenderSystemWrapper<>& wrapper){
		_context->renderSystems.destroy(wrapper.ID());
	}

	RenderSystems::RenderSystemRegistry& Renderer::renderSystems(){
		return _context->renderSystems;
	}

	const RenderSystems::RenderSystemRegistry& Renderer::renderSystems() const{
		return _context->renderSystems;
	}

	PipelineLayoutWrapper Renderer::createPipelineLayout(const Pipelines::PipelineLayoutConfigInfo& info){
		return PipelineLayoutWrapper(*_context, _context->pipelineLayoutRegistry.create(info));
	}

	void Renderer::destroyPipelineLayout(const PipelineLayoutWrapper& wrapper){
		_context->pipelineLayoutRegistry.destroy(wrapper.ID());
	}

	Pipelines::LayoutRegistry& Renderer::pipelineLayouts(){
		return _context->pipelineLayoutRegistry;
	}

	const Pipelines::LayoutRegistry& Renderer::pipelineLayouts() const{
		return _context->pipelineLayoutRegistry;
	}

	GraphicsPipelineWrapper Renderer::createGraphicsPipeline(const Pipelines::GraphicsPipelineConfigInfo& info){
		return GraphicsPipelineWrapper(*_context, _context->graphicsPipelineRegistry.create(info));
	}

	void Renderer::destroyGraphicsPipeline(const GraphicsPipelineWrapper& wrapper){
		_context->graphicsPipelineRegistry.destroy(wrapper.ID());
	}

	Pipelines::GraphicsPipelineRegistry& Renderer::graphicsPipelines(){
		return _context->graphicsPipelineRegistry;
	}

	const Pipelines::GraphicsPipelineRegistry& Renderer::graphicsPipelines() const{
		return _context->graphicsPipelineRegistry;
	}
}