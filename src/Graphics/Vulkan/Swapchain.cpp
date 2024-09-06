#include "Raindrop/Graphics/Vulkan/Swapchain.hpp"
#include "Raindrop/Graphics/Vulkan/RenderPass.hpp"
#include "Raindrop/Graphics/Context.hpp"

#include <vulkan/vk_enum_string_helper.h>

namespace Raindrop::Graphics::Vulkan{
	Swapchain::SwapchainData::SwapchainData(Context& context) :
		context{context},
		swapchain{VK_NULL_HANDLE},
		frames{}
	{}

	Swapchain::SwapchainData::~SwapchainData(){
		auto& device = context.getDevice();

		for (auto &frame : frames){
			if (frame.framebuffer != VK_NULL_HANDLE){
				vkDestroyFramebuffer(device.get(), frame.framebuffer, nullptr);
				frame.framebuffer = VK_NULL_HANDLE;
			}

			if (frame.imageView != VK_NULL_HANDLE){
				vkDestroyImageView(device.get(), frame.imageView, nullptr);
				frame.imageView = VK_NULL_HANDLE;
			}

			if (frame.imageAvailable != VK_NULL_HANDLE){
				vkDestroySemaphore(device.get(), frame.imageAvailable, nullptr);
				frame.imageAvailable = VK_NULL_HANDLE;
			}

			if (frame.imageFinished != VK_NULL_HANDLE){
				vkDestroySemaphore(device.get(), frame.imageFinished, nullptr);
				frame.imageFinished = VK_NULL_HANDLE;
			}

			if (frame.inFlightFence != VK_NULL_HANDLE){
				vkDestroyFence(device.get(), frame.inFlightFence, nullptr);
				frame.inFlightFence = VK_NULL_HANDLE;
			}
		}

		if (swapchain){
			vkDestroySwapchainKHR(device.get(), swapchain, nullptr);
			swapchain = VK_NULL_HANDLE;
		}

		frames.clear();
	}

	Swapchain::Swapchain(Context& context) : 
		_context{context},
		_swapchain{},
		_oldSwapchain{},
		_renderPass{},
		_currentFrame{0},
		_nextFrame{0},
		_frameCount{0},
		_extent{
			.width = 0,
			.height = 0
		},
		_presentMode{VK_PRESENT_MODE_IMMEDIATE_KHR},
		_surfaceFormat{
			.format = VK_FORMAT_UNDEFINED,
			.colorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR
		},
		_buildInfo{
			.frameCount{0},
			.extent{0, 0},
			.presentMode{VK_PRESENT_MODE_IMMEDIATE_KHR},
			.surfaceFormat{
				.format = VK_FORMAT_UNDEFINED,
				.colorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR
			},
		},
		_support{
			.capabilities{},
			.formats{},
			.presentModes{}
		},
		_clearColor{
			.uint32{0, 0, 0, 0}
		}
	{
		SPDLOG_LOGGER_INFO(_context.getLogger(), "Creating swapchain...");
		
		querySupport();

		_frameCount = findFrameCount();
		_extent = findExtent();
		_presentMode = findPresentMode();
		_surfaceFormat = findSurfaceFormat();
		
		createRenderPass();
		rebuildSwapchain();

		SPDLOG_LOGGER_INFO(_context.getLogger(), "Swapchain created with success !");
	}

	Swapchain::~Swapchain(){
		SPDLOG_LOGGER_INFO(_context.getLogger(), "Destroying swapchain...");

		_oldSwapchain.reset();
		_swapchain.reset();
		_renderPass.reset();
	}

	void Swapchain::querySupport(){
		querySupportCapabilities();
		querySupportFormats();
		querySupportPresentModes();
	}

	void Swapchain::querySupportCapabilities(){
		const auto& physicalDevice = _context.getPhysicalDevice();
		const auto& surface = _context.getSurface();

		if (vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice.get(), surface.get(), &_support.capabilities) != VK_SUCCESS){
			SPDLOG_LOGGER_ERROR(_context.getLogger(), "Failed to query surface swapchain capabilities");
			throw std::runtime_error("Failed to quert surface capabilities");
		}
	}

	void Swapchain::querySupportFormats(){
		const auto& physicalDevice = _context.getPhysicalDevice();
		const auto& surface = _context.getSurface();

		uint32_t count = 0;

		if (vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice.get(), surface.get(), &count, nullptr) != VK_SUCCESS){
			SPDLOG_LOGGER_ERROR(_context.getLogger(), "Failed to query surface swapchain formats");
			throw std::runtime_error("Failed to query surface swapchain formats");
		}

		if (count == 0){
			SPDLOG_LOGGER_ERROR(_context.getLogger(), "Could not find a suitable swapchain format");
			throw std::runtime_error("Could not find a suitable swapchain format");
		}

		_support.formats.resize(static_cast<std::size_t>(count));
		vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice.get(), surface.get(), &count, _support.formats.data());
	}

	void Swapchain::querySupportPresentModes(){
		const auto& physicalDevice = _context.getPhysicalDevice();
		const auto& surface = _context.getSurface();

		uint32_t count = 0;

		if (vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice.get(), surface.get(), &count, nullptr) != VK_SUCCESS){
			SPDLOG_LOGGER_ERROR(_context.getLogger(), "Failed to query surface swapchain present modes");
			throw std::runtime_error("Failed to query surface swapchain present modes");
		}

		if (count == 0){
			SPDLOG_LOGGER_ERROR(_context.getLogger(), "Could not find a suitable swapchain present mode");
			throw std::runtime_error("Could not find a suitable swapchain present mode");
		}

		_support.presentModes.resize(static_cast<std::size_t>(count));
		vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice.get(), surface.get(), &count, _support.presentModes.data());
	}

	void Swapchain::createRenderPass(){

		RenderPass::Builder builder(_context);
		// _renderPass = RenderPass::create() REGISTRY.emplace<RenderPass>();

		auto attachment = builder.addAttachment()
			.setFormat(_surfaceFormat.format)
			.setLoadOperation(VK_ATTACHMENT_LOAD_OP_CLEAR)
			.setStoreOperation(VK_ATTACHMENT_STORE_OP_STORE)
			.setStencilLoadOperation(VK_ATTACHMENT_LOAD_OP_DONT_CARE)
			.setStencilStoreOperation(VK_ATTACHMENT_STORE_OP_DONT_CARE)
			.setInitialLayout(VK_IMAGE_LAYOUT_UNDEFINED)
			.setFinalLayout(VK_IMAGE_LAYOUT_PRESENT_SRC_KHR);
		
		auto subpass = builder.addSubpass()
			.addColorAttachment(attachment, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
		
		auto dependency = builder.addDependency()
			.setSrcSubpass(RenderPass::SubpassDescription::External)
			.setSrcAccess(VK_ACCESS_NONE)
			.setSrcStage(VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT)
			.setDstSubpass(subpass)
			.setDstAccess(VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT)
			.setDstStage(VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT);
		
		_renderPass = RenderPass::Create(builder);
	}

	VkSurfaceFormatKHR Swapchain::findSurfaceFormat(){

		/**
		 * I thought it would be better to find the most suitable
		 * format instead of fallbacking on a random format
		 * if we fail to find the requested format
		 */
		
		const auto& wanted = _buildInfo.surfaceFormat;

		VkSurfaceFormatKHR bestFormat = _support.formats[0];
		uint32_t bestFormatScore = 0;

		for (const auto& format : _support.formats){
			uint32_t score = 0;

			score += int(format.colorSpace == wanted.colorSpace);
			score += int(format.format == wanted.format);

			if (score > bestFormatScore){
				bestFormat = format;
				bestFormatScore = score;
			}
		}

		return bestFormat;
	}

	VkPresentModeKHR Swapchain::findPresentMode(){
		const auto& wanted = _buildInfo.presentMode;

		for (const auto& mode : _support.presentModes){
			if (mode == wanted){
				return mode;
			}
		}

		return _support.presentModes[0];
	}

	VkExtent2D Swapchain::findExtent(){
		const auto& wanted = _buildInfo.extent;
		auto& capabilities = _support.capabilities;

		VkExtent2D extent;

		if (capabilities.currentExtent.width != UINT32_MAX){
			return capabilities.currentExtent;
		}

		extent.width = std::clamp(wanted.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
		extent.height = std::clamp(wanted.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

		return extent;
	}

	std::uint32_t Swapchain::findFrameCount(){
		const auto& capabilities = _support.capabilities;
		return std::clamp(_buildInfo.frameCount, capabilities.minImageCount, capabilities.maxImageCount == 0 ? UINT32_MAX : capabilities.maxImageCount);
	}


	Swapchain& Swapchain::wantExtent(VkExtent2D extent){
		_buildInfo.extent = extent;
		return *this;
	}

	Swapchain& Swapchain::wantFrameCount(uint32_t count){
		_buildInfo.frameCount = count;
		return *this;
	}

	Swapchain& Swapchain::wantPresentMode(VkPresentModeKHR presentMode){
		_buildInfo.presentMode = presentMode;
		return *this;
	}

	Swapchain& Swapchain::wantSurfaceFormat(VkSurfaceFormatKHR surfaceFormat){
		_buildInfo.surfaceFormat = surfaceFormat;
		return *this;
	}

	void Swapchain::rebuildSwapchain(){
		auto& device = _context.getDevice();

		SPDLOG_LOGGER_INFO(_context.getLogger(), "Rebuilding swapchain...");
		
		// maye change by vkQueueWaitIdle(device.presentQueue.queue);
		device.waitIdle();
		
		std::swap(_swapchain, _oldSwapchain);
		_swapchain = std::make_unique<SwapchainData>(_context);

		querySupport();

		VkSurfaceFormatKHR surfaceFormat = findSurfaceFormat();
		const bool hasFormatChanged = surfaceFormat.format != _surfaceFormat.format;
		const bool hasColorSpaceChanged = surfaceFormat.colorSpace != _surfaceFormat.colorSpace;

		if (hasColorSpaceChanged || hasFormatChanged){
			SPDLOG_LOGGER_ERROR(_context.getLogger(),
				"The swapchain surface format and / or color space has changed\n"
				"\t - Format (before/after) : {} / {}\n"
				"\t - Colorspace (before/after) : {} / {}",
				string_VkFormat(_surfaceFormat.format), string_VkFormat(surfaceFormat.format),
				string_VkColorSpaceKHR(_surfaceFormat.colorSpace), string_VkColorSpaceKHR(surfaceFormat.colorSpace)
			);
			throw std::runtime_error("The surface format and/or color space has changed");
		}

		_frameCount = findFrameCount();
		_extent = findExtent();
		_presentMode = findPresentMode();
		
		VkSwapchainCreateInfoKHR info{};
		info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		info.surface = _context.getSurface().get();
		info.oldSwapchain = _oldSwapchain != nullptr ? _oldSwapchain->swapchain : VK_NULL_HANDLE;
		info.preTransform = _support.capabilities.currentTransform;
		info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		info.presentMode = _presentMode;
		info.clipped = VK_TRUE;

		uint32_t queueFamilyIndices[] = {
			_context.getDevice().graphicsQueue.familyIndex,
			_context.getDevice().presentQueue.familyIndex,
		};

		if (queueFamilyIndices[0] != queueFamilyIndices[1]){
			info.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
			info.queueFamilyIndexCount = 2;
			info.pQueueFamilyIndices = queueFamilyIndices;
		} else {
			info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
			info.queueFamilyIndexCount = 0;
			info.pQueueFamilyIndices = nullptr;
		}
		
		info.minImageCount = _frameCount;
		info.imageFormat = _surfaceFormat.format;
		info.imageColorSpace = _surfaceFormat.colorSpace;
		info.imageExtent = _extent;
		info.imageArrayLayers = 1;
		info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

		if (vkCreateSwapchainKHR(device.get(), &info, nullptr, &_swapchain->swapchain) != VK_SUCCESS){
			SPDLOG_LOGGER_ERROR(_context.getLogger(), "Failed to build swapchain");
			throw std::runtime_error("Failed to build swapchain");
		}

		_oldSwapchain.reset();

		getSwapchainImages();
		createImageViews();
		createFramebuffers();
		createSyncObjects();

		_currentFrame = 0;
		SPDLOG_LOGGER_INFO(_context.getLogger(), "The swapchain has been rebuilt successfully !");
	}

	void Swapchain::getSwapchainImages(){
		auto& device = _context.getDevice();

		uint32_t imageCount = 0;
 		if (vkGetSwapchainImagesKHR(device.get(), _swapchain->swapchain, &imageCount, nullptr) != VK_SUCCESS){
			SPDLOG_LOGGER_ERROR(_context.getLogger(), "Failed to get swapchain images");
			throw std::runtime_error("Failed to get swapchain images");
		}

		if (imageCount != _frameCount){
			SPDLOG_LOGGER_INFO(_context.getLogger(), "The number of swapchain frame changed from {} to {}", _frameCount, imageCount);
		}

		_frameCount = imageCount;

		std::vector<VkImage> images(_frameCount);
		if (vkGetSwapchainImagesKHR(device.get(), _swapchain->swapchain, &_frameCount, images.data()) != VK_SUCCESS){
			SPDLOG_LOGGER_ERROR(_context.getLogger(), "Failed to get swapchain images");
			throw std::runtime_error("Failed to get swapchain images");
		}

		// forcefully reload everything to ensure proper destroy
		_swapchain->frames.clear();
		_swapchain->frames.resize(_frameCount);

		for (uint32_t i=0; i<_frameCount; i++){
			_swapchain->frames[i].image = images[i];
		}
	}

	void Swapchain::createImageViews(){
		auto& device = _context.getDevice();

		for (auto &frame : _swapchain->frames){
			VkImageViewCreateInfo viewInfo{};

			viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			viewInfo.image = frame.image;
			viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
			viewInfo.format = _surfaceFormat.format;

			viewInfo.subresourceRange = {
				.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
				.baseMipLevel = 0,
				.levelCount = 1,
				.baseArrayLayer = 0,
				.layerCount = 1
			};

			if (vkCreateImageView(device.get(), &viewInfo, nullptr, &frame.imageView) != VK_SUCCESS){
				SPDLOG_LOGGER_ERROR(_context.getLogger(), "Failed to create swapchain attachment image view");
				throw std::runtime_error("Failed to create swapchain attachment image view");
			}
		}
	}

	void Swapchain::createFramebuffers(){
		auto& device = _context.getDevice();

		for (auto &frame : _swapchain->frames){
			VkFramebufferCreateInfo framebufferInfo = {};

			framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
			framebufferInfo.renderPass = _renderPass->get();
			framebufferInfo.attachmentCount = 1;
			framebufferInfo.pAttachments = &frame.imageView;
			framebufferInfo.width = _extent.width;
			framebufferInfo.height = _extent.height;
			framebufferInfo.layers = 1;

			if (vkCreateFramebuffer(device.get(), &framebufferInfo, nullptr, &frame.framebuffer) != VK_SUCCESS){
				SPDLOG_LOGGER_ERROR(_context.getLogger(), "Failed to create swapchain framebuffer");
				throw std::runtime_error("Failed to create swapchain framebuffer");
			}
		}
	}

	void Swapchain::createSyncObjects(){
		auto& device = _context.getDevice();

		VkSemaphoreCreateInfo semaphoreInfo = {};
		semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

		VkFenceCreateInfo fenceInfo = {};
		fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

		for (auto &frame : _swapchain->frames) {
			if (vkCreateSemaphore(device.get(), &semaphoreInfo, nullptr, &frame.imageAvailable) != VK_SUCCESS){
				SPDLOG_LOGGER_ERROR(_context.getLogger(), "Failed to create swapchain attachment image available semahore");
				throw std::runtime_error("Failed to create swapchain attachment image available semahore");
			}

			if (vkCreateSemaphore(device.get(), &semaphoreInfo, nullptr, &frame.imageFinished) != VK_SUCCESS){
				SPDLOG_LOGGER_ERROR(_context.getLogger(), "Failed to create swapchain attachment image finished semahore");
				throw std::runtime_error("Failed to create swapchain attachment image finished semahore");
			}
			
			if (vkCreateFence(device.get(), &fenceInfo, nullptr, &frame.inFlightFence) != VK_SUCCESS){
				SPDLOG_LOGGER_ERROR(_context.getLogger(), "Failed to create swapchain attachment in flight fence");
				throw std::runtime_error("Failed to create swapchain attachment in flight fence");
			}
		}
	}

	void Swapchain::beginRenderPass(VkCommandBuffer commandBuffer){
		VkRenderPassBeginInfo renderPassBeginInfo{};

		renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassBeginInfo.renderPass = _renderPass->get();
		renderPassBeginInfo.framebuffer = _swapchain->frames[_currentFrame].framebuffer;

		renderPassBeginInfo.renderArea.offset = {0, 0};
		renderPassBeginInfo.renderArea.extent = _extent;

		VkClearValue clearValue = VkClearValue{_clearColor};

		renderPassBeginInfo.clearValueCount = 1;
		renderPassBeginInfo.pClearValues = &clearValue;

		vkCmdBeginRenderPass(commandBuffer, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

		VkViewport viewport{};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width =  _extent.width;
		viewport.height = _extent.height;
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;
		VkRect2D scissor{{0, 0}, _extent};

		vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
		vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
	}

	void Swapchain::endRenderPass(VkCommandBuffer commandBuffer){
    	vkCmdEndRenderPass(commandBuffer);
	}

	VkResult Swapchain::acquireNextImage(){
		auto& device = _context.getDevice();

		if (vkWaitForFences(device.get(), 1, &_swapchain->frames[_currentFrame].inFlightFence, VK_TRUE, UINT64_MAX) != VK_SUCCESS){
			SPDLOG_LOGGER_ERROR(_context.getLogger(), "Failed to wait for swapchain attachment in flight fence");
			throw std::runtime_error("Failed to wait for swapchain attachment in flight fence");
		}

		VkResult result = vkAcquireNextImageKHR(device.get(), _swapchain->swapchain, UINT64_MAX, _swapchain->frames[_currentFrame].imageAvailable, VK_NULL_HANDLE, &_nextFrame);
		return result;
	}

	VkResult Swapchain::submitCommandBuffer(std::vector<VkCommandBuffer> buffers){
		auto& device = _context.getDevice();
		
		if (_swapchain->frames[_nextFrame].imageInFlight != VK_NULL_HANDLE){

			if (vkWaitForFences(device.get(), 1, &_swapchain->frames[_nextFrame].imageInFlight, VK_TRUE, UINT64_MAX) != VK_SUCCESS){
				SPDLOG_LOGGER_ERROR(_context.getLogger(), "Failed to wait for swapchain attachment in flight fence");
				throw std::runtime_error("Failed to wait for swapchain attachment in flight fence");
			}
		}
		
		_swapchain->frames[_nextFrame].imageInFlight = _swapchain->frames[_currentFrame].inFlightFence;

		VkSemaphore waitSemaphores[] = {_swapchain->frames[_currentFrame].imageAvailable};
		VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
		VkSemaphore signalSemaphores[] = {_swapchain->frames[_currentFrame].imageFinished};

		VkSubmitInfo submitInfo = {};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitSemaphores = waitSemaphores;
		submitInfo.pWaitDstStageMask = waitStages;

		submitInfo.commandBufferCount = static_cast<uint32_t>(buffers.size());
		submitInfo.pCommandBuffers = buffers.data();

		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = signalSemaphores;

		if (vkResetFences(device.get(), 1, &_swapchain->frames[_currentFrame].inFlightFence) != VK_SUCCESS){
			SPDLOG_LOGGER_ERROR(_context.getLogger(), "Failed to reset swapchain frame in flight fence");
			throw std::runtime_error("Failed to reset swapchain frame in flight fence");
		}
		
		if (vkQueueSubmit(device.graphicsQueue.queue, 1, &submitInfo, _swapchain->frames[_currentFrame].inFlightFence) != VK_SUCCESS){
			SPDLOG_LOGGER_ERROR(_context.getLogger(), "Faile to submit graphics command buffer");
			throw std::runtime_error("Faile to submit graphics command buffer");
		}

		VkPresentInfoKHR presentInfo = {};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = signalSemaphores;

		VkSwapchainKHR swapChains[] = {_swapchain->swapchain};
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = swapChains;

		presentInfo.pImageIndices = &_nextFrame;

		VkResult result = vkQueuePresentKHR(device.presentQueue.queue, &presentInfo);

		_currentFrame = (_currentFrame + 1) % _frameCount;
		return result;
	}

	void Swapchain::setClearColor(VkClearColorValue color){
		_clearColor = color;
	}

	Swapchain::Frame& Swapchain::getFrameData(uint32_t id){
		return _swapchain->frames[id];
	}

	Swapchain::Frame& Swapchain::getCurrentFrameData(){
		return _swapchain->frames[_currentFrame];
	}

	std::vector<Swapchain::Frame>& Swapchain::getFramesData(){
		return _swapchain->frames;
	}

	uint32_t Swapchain::getFrameCount() const{
		return _frameCount;
	}

	uint32_t Swapchain::getCurrentFrameIndex() const{
		return _currentFrame;
	}

	uint32_t Swapchain::getNextFrameIndex() const{
		return _nextFrame;
	}


	const std::shared_ptr<RenderPass>& Swapchain::getRenderPass() const{
		return _renderPass;
	}
}
