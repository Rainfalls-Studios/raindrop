// #include <Raindrop/Graphics/Core/Swapchain.hpp>
// #include <Raindrop/Graphics/Context.hpp>
// #include <Raindrop/RenderPass.hpp>
// #include <Raindrop_internal/RenderPass.hpp>
// #include <Raindrop_internal/Format.hpp>
// #include <spdlog/spdlog.h>

#include <Raindrop/Graphics/Window/Swapchain.hpp>
#include <Raindrop/Graphics/Window/Context.hpp>
#include <Raindrop/Graphics/Core/Context.hpp>

#include <vulkan/vk_enum_string_helper.h>

#include <Raindrop/Exceptions/VulkanExceptions.hpp>

namespace Raindrop::Graphics::Window{
	Swapchain::SwapchainData::SwapchainData(Core::Context* context) :
		context{context},
		swapchain{VK_NULL_HANDLE},
		frames{}
	{}

	Swapchain::SwapchainData::~SwapchainData(){
		auto& device = context->device;
		auto& allocationCallbacks = context->allocationCallbacks;
		
		device.waitIdle();
		for (auto &frame : frames){
			if (frame.framebuffer != VK_NULL_HANDLE){
				vkDestroyFramebuffer(device.get(), frame.framebuffer, allocationCallbacks);
				frame.framebuffer = VK_NULL_HANDLE;
			}

			if (frame.imageView != VK_NULL_HANDLE){
				vkDestroyImageView(device.get(), frame.imageView, allocationCallbacks);
				frame.imageView = VK_NULL_HANDLE;
			}

			if (frame.imageAvailable != VK_NULL_HANDLE){
				vkDestroySemaphore(device.get(), frame.imageAvailable, allocationCallbacks);
				frame.imageAvailable = VK_NULL_HANDLE;
			}

			if (frame.imageFinished != VK_NULL_HANDLE){
				vkDestroySemaphore(device.get(), frame.imageFinished, allocationCallbacks);
				frame.imageFinished = VK_NULL_HANDLE;
			}

			if (frame.inFlightFence != VK_NULL_HANDLE){
				vkDestroyFence(device.get(), frame.inFlightFence, allocationCallbacks);
				frame.inFlightFence = VK_NULL_HANDLE;
			}
		}

		if (swapchain){
			vkDestroySwapchainKHR(device.get(), swapchain, allocationCallbacks);
			swapchain = VK_NULL_HANDLE;
		}

		context = nullptr;
		frames.clear();
	}

	Swapchain::Swapchain() noexcept : 
		_context{nullptr},
		_core{nullptr},
		_graphics{nullptr},
		_swapchain{},
		_oldSwapchain{},
		_renderPass{},
		_currentFrame{0},
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
	{}

	Swapchain::~Swapchain(){
		release();
	}

	void Swapchain::prepare(Context& context, Core::Context& core, Graphics::Context& graphics){
		_context = &context;
		_core = &core;
		_graphics = &graphics;

		querySupport();
	}

	void Swapchain::initialize(){
		
		_frameCount = findFrameCount();
		_extent = findExtent();
		_presentMode = findPresentMode();
		_surfaceFormat = findSurfaceFormat();
		
		createRenderPass();

		rebuildSwapchain();
	}

	void Swapchain::release(){
		_context->logger->info("Destroying swapchain...");

		_oldSwapchain.reset();
		_swapchain.reset();
		_renderPass.release();
		_graphics = nullptr;
		_core = nullptr;
		_context = nullptr;
	}

	void Swapchain::querySupport(){
		querySupportCapabilities();
		querySupportFormats();
		querySupportPresentModes();
	}

	void Swapchain::querySupportCapabilities(){
		const auto& physicalDevice = _core->physicalDevice;
		const auto& surface = _context->surface;

		Exceptions::checkVkOperation<VkPhysicalDevice>(
			vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice.get(), surface.get(), &_support.capabilities),
			"Failed to query surface swapchain capabilities",
			Exceptions::VulkanOperationType::QUERYING,
			_context->logger
		);
	}

	void Swapchain::querySupportFormats(){
		const auto& physicalDevice = _core->physicalDevice;
		const auto& surface = _context->surface;

		uint32_t count = 0;

		Exceptions::checkVkOperation<VkPhysicalDevice>(
			vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice.get(), surface.get(), &count, nullptr),
			"Failed to query surface swapchain formats",
			Exceptions::VulkanOperationType::QUERYING,
			_context->logger
		);

		_support.formats.resize(static_cast<std::size_t>(count));

		Exceptions::checkVkOperation<VkPhysicalDevice>(
			vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice.get(), surface.get(), &count, _support.formats.data()),
			"Failed to query surface swapchain formats",
			Exceptions::VulkanOperationType::QUERYING,
			_context->logger
		);
	}

	void Swapchain::querySupportPresentModes(){
		const auto& physicalDevice = _core->physicalDevice;
		const auto& surface = _context->surface;

		uint32_t count = 0;

		Exceptions::checkVkOperation<VkPhysicalDevice>(
			vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice.get(), surface.get(), &count, nullptr),
			"Failed to query surface swapchain present modes",
			Exceptions::VulkanOperationType::QUERYING,
			_context->logger
		);

		_support.presentModes.resize(static_cast<std::size_t>(count));

		Exceptions::checkVkOperation<VkPhysicalDevice>(
			vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice.get(), surface.get(), &count, _support.presentModes.data()),
			"Failed to query surface swapchain present modes",
			Exceptions::VulkanOperationType::QUERYING,
			_context->logger
		);
	}

	void Swapchain::createRenderPass(){
		_renderPass.prepare(*_graphics);
		auto attachment = _renderPass.addAttachment()
			.setFormat(_surfaceFormat.format)
			.setLoadOperation(VK_ATTACHMENT_LOAD_OP_CLEAR)
			.setStoreOperation(VK_ATTACHMENT_STORE_OP_STORE)
			.setStencilLoadOperation(VK_ATTACHMENT_LOAD_OP_DONT_CARE)
			.setStencilStoreOperation(VK_ATTACHMENT_STORE_OP_DONT_CARE)
			.setInitialLayout(VK_IMAGE_LAYOUT_UNDEFINED)
			.setFinalLayout(VK_IMAGE_LAYOUT_PRESENT_SRC_KHR);
		
		auto subpass = _renderPass.addSubpass()
			.addColorAttachment(attachment, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
		
		auto dependency = _renderPass.addDependency()
			.setSrcSubpass(RenderPass::SubpassDescription::External)
			.setSrcAccess(VK_ACCESS_NONE)
			.setSrcStage(VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT)
			.setDstSubpass(subpass)
			.setDstAccess(VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT)
			.setDstStage(VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT);
		
		_renderPass.initialize();
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
		auto& device = _core->device;

		_context->logger->info("Rebuilding swapchain...");
		
		// maye change by vkQueueWaitIdle(device.presentQueue.queue);
		device.waitIdle();
		
		std::swap(_swapchain, _oldSwapchain);
		_swapchain = std::make_unique<SwapchainData>(_core);

		querySupport();

		VkSurfaceFormatKHR surfaceFormat = findSurfaceFormat();
		const bool hasFormatChanged = surfaceFormat.format != _surfaceFormat.format;
		const bool hasColorSpaceChanged = surfaceFormat.colorSpace != _surfaceFormat.colorSpace;

		if (hasColorSpaceChanged || hasFormatChanged){
			_context->logger->error(
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
		info.surface = _context->surface.get();
		info.oldSwapchain = _oldSwapchain != nullptr ? _oldSwapchain->swapchain : VK_NULL_HANDLE;
		info.preTransform = _support.capabilities.currentTransform;
		info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		info.presentMode = _presentMode;
		info.clipped = VK_TRUE;
		
		info.minImageCount = _frameCount;
		info.imageFormat = _surfaceFormat.format;
		info.imageColorSpace = _surfaceFormat.colorSpace;
		info.imageExtent = _extent;
		info.imageArrayLayers = 1;
		info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

		Exceptions::checkVkCreation<VkSwapchainKHR>(
			vkCreateSwapchainKHR(device.get(), &info, _core->allocationCallbacks, &_swapchain->swapchain),
			"Failed to create swapchain",
			_context->logger
		);

		_oldSwapchain.reset();

		getSwapchainImages();
		createImageViews();
		createFramebuffers();
		createSyncObjects();

		_currentFrame = 0;
		_context->logger->info("The vulkan swapchain has been rebuilt without any critical error");
	}


	void Swapchain::getSwapchainImages(){
		auto& device = _core->device;

		uint32_t imageCount = 0;
		Exceptions::checkVkOperation<VkSwapchainKHR>(
			vkGetSwapchainImagesKHR(device.get(), _swapchain->swapchain, &imageCount, nullptr),
			"Failed to get swapchain images",
			Exceptions::VulkanOperationType::QUERYING,
			_context->logger
		);

		if (imageCount != _frameCount){
			_context->logger->info("The number of swapchain frame changed from {} to {}", _frameCount, imageCount);
		}

		_frameCount = imageCount;

		std::vector<VkImage> images(_frameCount);
		Exceptions::checkVkOperation<VkSwapchainKHR>(
			vkGetSwapchainImagesKHR(device.get(), _swapchain->swapchain, &_frameCount, images.data()),
			"Failed to get swapchain images",
			Exceptions::VulkanOperationType::QUERYING,
			_context->logger
		);

		// forcefully reload everything to ensure proper destroy
		_swapchain->frames.clear();
		_swapchain->frames.resize(_frameCount);

		for (uint32_t i=0; i<_frameCount; i++){
			_swapchain->frames[i].image = images[i];
		}
	}

	void Swapchain::createImageViews(){
		auto& device = _core->device;
		auto& allocationCallbacks = _core->allocationCallbacks;

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

			Exceptions::checkVkCreation<VkImageView>(
				vkCreateImageView(device.get(), &viewInfo, allocationCallbacks, &frame.imageView),
				"Failed to create swapchain attachment image view",
				_context->logger
			);
		}
	}

	void Swapchain::createFramebuffers(){
		auto& device = _core->device;
		auto& allocationCallbacks = _core->allocationCallbacks;

		for (auto &frame : _swapchain->frames){
			VkFramebufferCreateInfo framebufferInfo = {};

			framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
			framebufferInfo.renderPass = _renderPass.get();
			framebufferInfo.attachmentCount = 1;
			framebufferInfo.pAttachments = &frame.imageView;
			framebufferInfo.width = _extent.width;
			framebufferInfo.height = _extent.height;
			framebufferInfo.layers = 1;

			Exceptions::checkVkCreation<VkFramebuffer>(
				vkCreateFramebuffer(device.get(), &framebufferInfo, allocationCallbacks, &frame.framebuffer),
				"Failed to create swapchain framebuffer",
				_context->logger
			);
		}
	}

	void Swapchain::createSyncObjects(){
		auto& device = _core->device;
		auto& allocationCallbacks = _core->allocationCallbacks;

		VkSemaphoreCreateInfo semaphoreInfo = {};
		semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

		VkFenceCreateInfo fenceInfo = {};
		fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

		for (auto &frame : _swapchain->frames) {
			Exceptions::checkVkCreation<VkSemaphore>(
				vkCreateSemaphore(device.get(), &semaphoreInfo, allocationCallbacks, &frame.imageAvailable),
				"Failed to create swapchain attachment image available semahore",
				_context->logger
			);

			Exceptions::checkVkCreation<VkSemaphore>(
				vkCreateSemaphore(device.get(), &semaphoreInfo, allocationCallbacks, &frame.imageFinished),
				"Failed to create swapchain attachment image finished semahore",
				_context->logger
			);
			
			Exceptions::checkVkCreation<VkFence>(
				vkCreateFence(device.get(), &fenceInfo, allocationCallbacks, &frame.inFlightFence),
				"Faile dto create swapchain attachment in flight fence",
				_context->logger
			);
		}
	}

}


// namespace Raindrop::Graphics::Window{
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
// 			vkCreateSwapchainKHR(device.get(), &info, allocationCallbacks, &_swapchain->swapchain),
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
// 			_swapchain->frames.resize(_frameCount);
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

// 		for (auto &f : _swapchain->frames){
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
// 			vkGetSwapchainImagesKHR(device.get(), _swapchain->swapchain, &_frameCount, nullptr),
// 			"Failed to get swapchain images",
// 			Exceptions::VulkanOperationType::QUERYING
// 		);

// 		std::vector<VkImage> images(_frameCount);
// 		Exceptions::checkVkOperation<VkSwapchainKHR>(
// 			vkGetSwapchainImagesKHR(device.get(), _swapchain->swapchain, &_frameCount, images.data()),
// 			"Failed to get swapchain images",
// 			Exceptions::VulkanOperationType::QUERYING
// 		);

// 		_swapchain->frames.resize(_frameCount);

// 		for (uint32_t i=0; i<_frameCount; i++){
// 			_swapchain->frames[i].image = images[i];
// 		}
// 	}

// 	void Swapchain::createFramebuffers(){
// 		auto& device = _context.getDevice();
// 		auto& allocationCallbacks = _context.getAllocationCallbacks();

// 		for (auto &f : _swapchain->frames){
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

// 		for (auto &f : _swapchain->frames) {
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
// 			vkWaitForFences(device.get(), 1, &_swapchain->frames[_currentFrame].inFlightFence, VK_TRUE, UINT64_MAX),
// 			"Failed to wait for swapchain attachment in flight fence",
// 			Exceptions::VulkanOperationType::WAIT
// 		);

// 		VkResult result = vkAcquireNextImageKHR(device.get(), _swapchain->swapchain, UINT64_MAX, _swapchain->frames[_currentFrame].imageAvailable, VK_NULL_HANDLE, &index);
// 		return result;
// 	}

// 	VkResult Swapchain::submitCommandBuffer(VkCommandBuffer* buffers){
// 		auto& device = _context.getDevice();
// 		auto& allocationCallbacks = _context.getAllocationCallbacks();

// 		if (_swapchain->frames[_currentFrame].imageInFlight != VK_NULL_HANDLE){
// 			Exceptions::checkVkOperation<VkFence>(
// 				vkWaitForFences(device.get(), 1, &_swapchain->frames[_currentFrame].imageInFlight, VK_TRUE, UINT64_MAX),
// 				"Failed to wait for swapchain attachment in flight fence",
// 				Exceptions::VulkanOperationType::WAIT
// 			);
// 		}
		
// 		_swapchain->frames[_currentFrame].imageInFlight = _swapchain->frames[_currentFrame].inFlightFence;

// 		VkSubmitInfo submitInfo = {};
// 		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

// 		VkSemaphore waitSemaphores[] = {_swapchain->frames[_currentFrame].imageAvailable};
// 		VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
// 		submitInfo.waitSemaphoreCount = 1;
// 		submitInfo.pWaitSemaphores = waitSemaphores;
// 		submitInfo.pWaitDstStageMask = waitStages;

// 		submitInfo.commandBufferCount = 1;
// 		submitInfo.pCommandBuffers = buffers;

// 		VkSemaphore signalSemaphores[] = {_swapchain->frames[_currentFrame].imageFinished};
// 		submitInfo.signalSemaphoreCount = 1;
// 		submitInfo.pSignalSemaphores = signalSemaphores;

// 		Exceptions::checkVkOperation<VkFence>(
// 			vkResetFences(device.get(), 1, &_swapchain->frames[_currentFrame].inFlightFence),
// 			"Failed to reset swapchain attachment in flight fence",
// 			Exceptions::VulkanOperationType::RESET
// 		);

// 		Exceptions::checkVkOperation<VkCommandBuffer>(
// 			vkQueueSubmit(_context.frame.getQueue()->get(), 1, &submitInfo, _swapchain->frames[_currentFrame].inFlightFence),
// 			"Faile to submit graphics command buffer",
// 			Exceptions::VulkanOperationType::SUBMIT
// 		);

// 		VkPresentInfoKHR presentInfo = {};
// 		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

// 		presentInfo.waitSemaphoreCount = 1;
// 		presentInfo.pWaitSemaphores = signalSemaphores;

// 		VkSwapchainKHR swapChains[] = {_swapchain->swapchain};
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
// 		renderPassBeginInfo.framebuffer = _swapchain->frames[_currentFrame].framebuffer;

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
// 		return _swapchain->frames[id];
// 	}

// 	Swapchain::Frame& Swapchain::getCurrentFrameData(){
// 		return _swapchain->frames[currentFrame()];
// 	}

// 	std::vector<Swapchain::Frame>& Swapchain::getFramesData(){
// 		return _swapchain->frames;
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