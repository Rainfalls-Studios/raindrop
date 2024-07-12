// #include <Raindrop_internal/Graphics/Core/Swapchain.hpp>
// #include <Raindrop_internal/Graphics/Context.hpp>
// #include <Raindrop/RenderPass.hpp>
// #include <Raindrop_internal/RenderPass.hpp>
// #include <Raindrop_internal/Format.hpp>
// #include <spdlog/spdlog.h>

// #include <Raindrop/Exceptions/VulkanExceptions.hpp>

// namespace Raindrop::Internal::Graphics::Core{
// 	Swapchain::Swapchain(Context& context) :
// 			_context{context},
// 			_swapchainSupport{},
// 			_swapchain{nullptr},
// 			_oldSwapchain{nullptr},
// 			_renderPass{Raindrop::RenderPass::Create(context.getInternalContext().getInterface())},
// 			_currentFrame{0},
// 			_imageFormat{VK_FORMAT_UNDEFINED},
// 			_frameCount{0},
// 			_extent{0, 0},
// 			_presentMode{VK_PRESENT_MODE_IMMEDIATE_KHR},
// 			_surfaceFormat{VK_FORMAT_UNDEFINED}{
// 		_context.getLogger()->info("Constructing vulkan swapchain...");

// 		_wantedExtent = {0, 0};
// 		_wantedFrameCount = 2;
// 		_wantedPresentMode = VK_PRESENT_MODE_FIFO_KHR;
// 		_wantedSurfaceFormat = {VK_FORMAT_B8G8R8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR};
// 		_clearColor = {0.f, 0.f, 0.f, 0.f};

// 		_swapchainSupport = _context.getPhysicalDevice().getSwapchainSupport(_context.getWindow().surface());

// 		findSurfaceFormat();
// 		createRenderPass();	
// 		rebuildSwapchain();
// 	}

// 	Swapchain::~Swapchain(){
// 		_context.getLogger()->info("Destroying vulkan swapchain...");
// 		auto device = _context.getDevice().get();
// 		auto allocationCallbacks = _context.getAllocationCallbacks();

// 		_oldSwapchain.reset();
// 		_swapchain.reset();

// 		_renderPass.release();
// 	}
	
// 	void Swapchain::setExtent(VkExtent2D extent){
// 		_wantedExtent = extent;
// 	}

// 	void Swapchain::setFrameCount(uint32_t count){
// 		_wantedFrameCount = count;
// 	}

// 	void Swapchain::setPresentMode(VkPresentModeKHR presentMode){
// 		_wantedPresentMode = presentMode;
// 	}

// 	void Swapchain::setSurfaceFormat(VkSurfaceFormatKHR surfaceFormat){
// 		_wantedSurfaceFormat = surfaceFormat;
// 	}

// 	void Swapchain::rebuildSwapchain(){
// 		auto& device = _context.getDevice();
// 		auto& allocationCallbacks = _context.getAllocationCallbacks();

// 		_context.getLogger()->info("Rebuilding vulkan swapchain...");
// 		device.waitIdle();
		
// 		std::swap(_swapchain, _oldSwapchain);
// 		_swapchain = std::make_unique<SwapchainData>(_context);

// 		_swapchainSupport = _context.getPhysicalDevice().getSwapchainSupport(_context.getWindow().surface());
// 		auto surfaceFormat = _surfaceFormat;

// 		findSurfaceFormat();
// 		bool hasColorSpaceChanged = surfaceFormat.colorSpace != _surfaceFormat.colorSpace;
// 		bool hasFormatChanged = surfaceFormat.format != _surfaceFormat.format;

// 		if (hasColorSpaceChanged || hasFormatChanged){
// 			_context.getLogger()->error("The swapchain surface format and / or color space has changed");
// 			throw std::runtime_error("The surface format and/or color space has changed");
// 		}

// 		findPresentMode();
// 		findFrameCount();
// 		findExtent();
		
// 		VkSwapchainCreateInfoKHR info{};
// 		info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
// 		info.surface = _context.getWindow().surface();
// 		info.oldSwapchain = _oldSwapchain != nullptr ? _oldSwapchain->_swapchain : VK_NULL_HANDLE;
// 		info.preTransform = _swapchainSupport.capabilities.currentTransform;
// 		info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
// 		info.presentMode = _presentMode;
// 		info.clipped = VK_TRUE;
		
// 		info.minImageCount = _frameCount;
// 		info.imageFormat = _surfaceFormat.format;
// 		info.imageColorSpace = _surfaceFormat.colorSpace;
// 		info.imageExtent = _extent;
// 		info.imageArrayLayers = 1;
// 		info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

// 		Exceptions::checkVkCreation<VkSwapchainKHR>(
// 			vkCreateSwapchainKHR(device.get(), &info, allocationCallbacks, &_swapchain->_swapchain),
// 			"Failed to create swapchain"
// 		);

// 		getSwapchainImages();
// 		createImageViews();
// 		createFramebuffers();
// 		createSyncObjects();

// 		_currentFrame = 0;
// 		_context.getLogger()->info("The vulkan swapchain has been rebuilt without any critical error");
// 	}

// 	void Swapchain::findSurfaceFormat(){
// 		for (auto& f : _swapchainSupport.formats){
// 			if (f.colorSpace == _wantedSurfaceFormat.colorSpace && f.format == _wantedSurfaceFormat.format){
// 				_surfaceFormat = _wantedSurfaceFormat;
// 				_imageFormat = _surfaceFormat.format;
// 				return;
// 			}
// 		}
// 		_surfaceFormat = _swapchainSupport.formats[0];
// 		_imageFormat = _surfaceFormat.format;
// 	}

// 	void Swapchain::findPresentMode(){
// 		for (auto& p : _swapchainSupport.presentModes){
// 			if (p == _wantedPresentMode){
// 				_presentMode = _wantedPresentMode;
// 			}
// 		}
// 		_presentMode = _swapchainSupport.presentModes[0];
// 	}

// 	void Swapchain::findExtent(){
// 		auto& capabilities = _swapchainSupport.capabilities;

// 		// if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()){
// 		// 	printf("aa\n");
// 		// 	_extent = capabilities.currentExtent;
// 		// 	return;
// 		// }

// 		_extent.width = std::clamp(_wantedExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
// 		_extent.height = std::clamp(_wantedExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

// 		return;
// 	}

// 	void Swapchain::findFrameCount(){
// 		auto& capabilities = _swapchainSupport.capabilities;

// 		uint32_t oldFrameCount = _frameCount;
// 		_frameCount = std::clamp(_wantedFrameCount, capabilities.minImageCount, capabilities.maxImageCount == 0 ? std::numeric_limits<uint32_t>::max() : capabilities.maxImageCount);

// 		if (oldFrameCount != _frameCount){
// 			_swapchain->_frames.resize(_frameCount);
// 		}
// 	}

// 	void Swapchain::createRenderPass(){
// 		auto& device = _context.getDevice();
// 		auto& allocationCallbacks = _context.getAllocationCallbacks();

// 		auto& attachment = _renderPass.addAttachment()
// 			.setFormat(FormatToRaindrop(_imageFormat))
// 			.setLoadOperation(Raindrop::RenderPass::AttachmentDescription::Operation::CLEAR)
// 			.setStoreOperation(Raindrop::RenderPass::AttachmentDescription::Operation::STORE)
// 			.setStencilLoadOperation(Raindrop::RenderPass::AttachmentDescription::Operation::DONT_LOAD)
// 			.setStencilStoreOperation(Raindrop::RenderPass::AttachmentDescription::Operation::DONT_STORE)
// 			.setInitialLayout(Raindrop::Texture::Layout::UNDEFINED)
// 			.setFinalLayout(Raindrop::Texture::Layout::PRESENT_SRC);

// 		auto& subpass = _renderPass.addSubpass()
// 			.addColorAttachment(
// 				{
// 					.attachment = attachment,
// 					.layout = Raindrop::Texture::Layout::COLOR_ATTACHMENT_OPTIMAL
// 				}
// 			);
		
// 		auto& dependency = _renderPass.addDependency()
// 			.setSrcSubpass(Raindrop::RenderPass::Subpass::External)
// 			.setSrcAccess(Raindrop::RenderPass::Access::NONE)
// 			.setSrcStage(Raindrop::Pipeline::Stage::COLOR_ATTACHMENT_OUTPUT)
// 			.setDstSubpass(subpass)
// 			.setDstAccess(Raindrop::RenderPass::Access::COLOR_ATTACHMENT_WRITE)
// 			.setDstStage(Raindrop::Pipeline::Stage::COLOR_ATTACHMENT_OUTPUT);
		

// 		_renderPass.initialize();
// 	}

// 	void Swapchain::createImageViews(){
// 		auto& device = _context.getDevice();
// 		auto& allocationCallbacks = _context.getAllocationCallbacks();

// 		for (auto &f : _swapchain->_frames){
// 			VkImageViewCreateInfo viewInfo{};
// 			viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
// 			viewInfo.image = f.image;
// 			viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
// 			viewInfo.format = _imageFormat;
// 			viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
// 			viewInfo.subresourceRange.baseMipLevel = 0;
// 			viewInfo.subresourceRange.levelCount = 1;
// 			viewInfo.subresourceRange.baseArrayLayer = 0;
// 			viewInfo.subresourceRange.layerCount = 1;

// 			Exceptions::checkVkCreation<VkImageView>(
// 				vkCreateImageView(device.get(), &viewInfo, allocationCallbacks, &f.imageView),
// 				"Failed to create swapchain attachment image view"
// 			);
// 		}
// 	}

// 	void Swapchain::getSwapchainImages(){
// 		auto& device = _context.getDevice();

// 		Exceptions::checkVkOperation<VkSwapchainKHR>(
// 			vkGetSwapchainImagesKHR(device.get(), _swapchain->_swapchain, &_frameCount, nullptr),
// 			"Failed to get swapchain images",
// 			Exceptions::VulkanOperationType::QUERYING
// 		);

// 		std::vector<VkImage> images(_frameCount);
// 		Exceptions::checkVkOperation<VkSwapchainKHR>(
// 			vkGetSwapchainImagesKHR(device.get(), _swapchain->_swapchain, &_frameCount, images.data()),
// 			"Failed to get swapchain images",
// 			Exceptions::VulkanOperationType::QUERYING
// 		);

// 		_swapchain->_frames.resize(_frameCount);

// 		for (uint32_t i=0; i<_frameCount; i++){
// 			_swapchain->_frames[i].image = images[i];
// 		}
// 	}

// 	void Swapchain::createFramebuffers(){
// 		auto& device = _context.getDevice();
// 		auto& allocationCallbacks = _context.getAllocationCallbacks();

// 		for (auto &f : _swapchain->_frames){
// 			VkFramebufferCreateInfo framebufferInfo = {};
// 			framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
// 			framebufferInfo.renderPass = _renderPass.getImpl()->internal->get();
// 			framebufferInfo.attachmentCount = 1;
// 			framebufferInfo.pAttachments = &f.imageView;
// 			framebufferInfo.width = _extent.width;
// 			framebufferInfo.height = _extent.height;
// 			framebufferInfo.layers = 1;

// 			Exceptions::checkVkCreation<VkFramebuffer>(
// 				vkCreateFramebuffer(device.get(), &framebufferInfo, allocationCallbacks, &f.framebuffer),
// 				"Failed to create swapchain framebuffer"
// 			);
// 		}
// 	}

// 	void Swapchain::createSyncObjects(){
// 		auto& device = _context.getDevice();
// 		auto& allocationCallbacks = _context.getAllocationCallbacks();

// 		VkSemaphoreCreateInfo semaphoreInfo = {};
// 		semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

// 		VkFenceCreateInfo fenceInfo = {};
// 		fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
// 		fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

// 		for (auto &f : _swapchain->_frames) {
// 			Exceptions::checkVkCreation<VkSemaphore>(
// 				vkCreateSemaphore(device.get(), &semaphoreInfo, allocationCallbacks, &f.imageAvailable),
// 				"Failed to create swapchain attachment image available semahore"
// 			);

// 			Exceptions::checkVkCreation<VkSemaphore>(
// 				vkCreateSemaphore(device.get(), &semaphoreInfo, allocationCallbacks, &f.imageFinished),
// 				"Failed to create swapchain attachment image finished semahore"
// 			);
			
// 			Exceptions::checkVkCreation<VkFence>(
// 				vkCreateFence(device.get(), &fenceInfo, allocationCallbacks, &f.inFlightFence),
// 				"Faile dto create swapchain attachment in flight fence"
// 			);
// 		}
// 	}
	
// 	VkResult Swapchain::acquireNextImage(){
// 		auto& device = _context.getDevice();
// 		uint32_t index = _currentFrame;

// 		Exceptions::checkVkOperation<VkFence>(
// 			vkWaitForFences(device.get(), 1, &_swapchain->_frames[_currentFrame].inFlightFence, VK_TRUE, UINT64_MAX),
// 			"Failed to wait for swapchain attachment in flight fence",
// 			Exceptions::VulkanOperationType::WAIT
// 		);

// 		VkResult result = vkAcquireNextImageKHR(device.get(), _swapchain->_swapchain, UINT64_MAX, _swapchain->_frames[_currentFrame].imageAvailable, VK_NULL_HANDLE, &index);
// 		return result;
// 	}

// 	VkResult Swapchain::submitCommandBuffer(VkCommandBuffer* buffers){
// 		auto& device = _context.getDevice();
// 		auto& allocationCallbacks = _context.getAllocationCallbacks();

// 		if (_swapchain->_frames[_currentFrame].imageInFlight != VK_NULL_HANDLE){
// 			Exceptions::checkVkOperation<VkFence>(
// 				vkWaitForFences(device.get(), 1, &_swapchain->_frames[_currentFrame].imageInFlight, VK_TRUE, UINT64_MAX),
// 				"Failed to wait for swapchain attachment in flight fence",
// 				Exceptions::VulkanOperationType::WAIT
// 			);
// 		}
		
// 		_swapchain->_frames[_currentFrame].imageInFlight = _swapchain->_frames[_currentFrame].inFlightFence;

// 		VkSubmitInfo submitInfo = {};
// 		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

// 		VkSemaphore waitSemaphores[] = {_swapchain->_frames[_currentFrame].imageAvailable};
// 		VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
// 		submitInfo.waitSemaphoreCount = 1;
// 		submitInfo.pWaitSemaphores = waitSemaphores;
// 		submitInfo.pWaitDstStageMask = waitStages;

// 		submitInfo.commandBufferCount = 1;
// 		submitInfo.pCommandBuffers = buffers;

// 		VkSemaphore signalSemaphores[] = {_swapchain->_frames[_currentFrame].imageFinished};
// 		submitInfo.signalSemaphoreCount = 1;
// 		submitInfo.pSignalSemaphores = signalSemaphores;

// 		Exceptions::checkVkOperation<VkFence>(
// 			vkResetFences(device.get(), 1, &_swapchain->_frames[_currentFrame].inFlightFence),
// 			"Failed to reset swapchain attachment in flight fence",
// 			Exceptions::VulkanOperationType::RESET
// 		);

// 		Exceptions::checkVkOperation<VkCommandBuffer>(
// 			vkQueueSubmit(_context.frame.getQueue()->get(), 1, &submitInfo, _swapchain->_frames[_currentFrame].inFlightFence),
// 			"Faile to submit graphics command buffer",
// 			Exceptions::VulkanOperationType::SUBMIT
// 		);

// 		VkPresentInfoKHR presentInfo = {};
// 		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

// 		presentInfo.waitSemaphoreCount = 1;
// 		presentInfo.pWaitSemaphores = signalSemaphores;

// 		VkSwapchainKHR swapChains[] = {_swapchain->_swapchain};
// 		presentInfo.swapchainCount = 1;
// 		presentInfo.pSwapchains = swapChains;

// 		presentInfo.pImageIndices = &_currentFrame;

// 		VkResult result = vkQueuePresentKHR(_context.frame.getQueue()->get(), &presentInfo);

// 		_currentFrame = (_currentFrame + 1) % _frameCount;
// 		return result;
// 	}

// 	uint32_t Swapchain::frameCount() const{
// 		return _frameCount;
// 	}
	
// 	const Raindrop::RenderPass& Swapchain::renderPass() const{
// 		return _renderPass;
// 	}

// 	uint32_t Swapchain::currentFrame() const{
// 		return _currentFrame;
// 	}

// 	void Swapchain::beginRenderPass(VkCommandBuffer commandBuffer){
// 		VkRenderPassBeginInfo renderPassBeginInfo{};

// 		renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
// 		renderPassBeginInfo.renderPass = _renderPass.getImpl()->internal->get();
// 		renderPassBeginInfo.framebuffer = _swapchain->_frames[_currentFrame].framebuffer;

// 		renderPassBeginInfo.renderArea.offset = {0, 0};
// 		renderPassBeginInfo.renderArea.extent = _extent;

// 		VkClearValue clearValue = VkClearValue{_clearColor};

// 		renderPassBeginInfo.clearValueCount = 1;
// 		renderPassBeginInfo.pClearValues = &clearValue;

// 		vkCmdBeginRenderPass(commandBuffer, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

// 		VkViewport viewport{};
// 		viewport.x = 0.0f;
// 		viewport.y = 0.0f;
// 		viewport.width =  _extent.width;
// 		viewport.height = _extent.height;
// 		viewport.minDepth = 0.0f;
// 		viewport.maxDepth = 1.0f;
// 		VkRect2D scissor{{0, 0}, _extent};

// 		vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
// 		vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
// 	}

// 	void Swapchain::endRenderPass(VkCommandBuffer commandBuffer){
//     	vkCmdEndRenderPass(commandBuffer);
// 	}

// 	void Swapchain::setClearColor(VkClearColorValue color){
// 		_clearColor = color;
// 	}
	
// 	Swapchain::Frame& Swapchain::getFrameData(uint32_t id){
// 		return _swapchain->_frames[id];
// 	}

// 	Swapchain::Frame& Swapchain::getCurrentFrameData(){
// 		return _swapchain->_frames[currentFrame()];
// 	}

// 	std::vector<Swapchain::Frame>& Swapchain::getFramesData(){
// 		return _swapchain->_frames;
// 	}
	
// 	Swapchain::SwapchainData::~SwapchainData(){
// 		auto& device = _context.getDevice();
// 		auto& allocationCallbacks = _context.getAllocationCallbacks();

// 		device.waitIdle();
// 		for (auto &frame : _frames){
// 			if (frame.framebuffer != VK_NULL_HANDLE) vkDestroyFramebuffer(device.get(), frame.framebuffer, allocationCallbacks);
// 			if (frame.imageView != VK_NULL_HANDLE) vkDestroyImageView(device.get(), frame.imageView, allocationCallbacks);
// 			if (frame.imageAvailable != VK_NULL_HANDLE) vkDestroySemaphore(device.get(), frame.imageAvailable, allocationCallbacks);
// 			if (frame.imageFinished != VK_NULL_HANDLE) vkDestroySemaphore(device.get(), frame.imageFinished, allocationCallbacks);
// 			if (frame.inFlightFence != VK_NULL_HANDLE) vkDestroyFence(device.get(), frame.inFlightFence, allocationCallbacks);
// 		}
// 		if (_swapchain) vkDestroySwapchainKHR(device.get(), _swapchain, allocationCallbacks);
// 	}

// 	Swapchain::SwapchainData::SwapchainData(Context& context) : _context{context}{}
// }