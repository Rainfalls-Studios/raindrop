#include <Raindrop/Renderer/Scene/Framebuffer.hpp>
#include <Raindrop/Renderer/Context.hpp>

#include <spdlog/spdlog.h>

namespace Raindrop::Renderer::Scene{
	Framebuffer::Attachment::Attachment(Context& context) : 
		context{context},
		imageView{VK_NULL_HANDLE},
		image{VK_NULL_HANDLE},
		memory{VK_NULL_HANDLE}{}

	Framebuffer::Attachment::~Attachment(){
		const auto& device = context.renderer.device.get();
		const auto& allocationCallbacks = context.renderer.allocationCallbacks;

		if (imageView != VK_NULL_HANDLE){
			vkDestroyImageView(device, imageView, allocationCallbacks);
			imageView = VK_NULL_HANDLE;
		}

		if (memory != VK_NULL_HANDLE){
			vkFreeMemory(device, memory, allocationCallbacks);
			memory = VK_NULL_HANDLE;
		}

		if (image != VK_NULL_HANDLE){
			vkDestroyImage(device, image, allocationCallbacks);
			image = VK_NULL_HANDLE;
		}
	}

	void Framebuffer::Attachment::create(const VkImageCreateInfo& imageInfo, VkImageViewCreateInfo imageViewInfo){
		auto& device = context.renderer.device;
		auto& allocationCallbacks = context.renderer.allocationCallbacks;

		if (vkCreateImage(device.get(), &imageInfo, allocationCallbacks, &image) != VK_SUCCESS){
			throw std::runtime_error("Failed to create attachment");
		}

		VkMemoryRequirements requirements;
		vkGetImageMemoryRequirements(device.get(), image, &requirements);

		VkMemoryAllocateInfo allocationInfo{};
		allocationInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocationInfo.memoryTypeIndex = device.findMemoryType(requirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
		allocationInfo.allocationSize = requirements.size;

		if (vkAllocateMemory(device.get(), &allocationInfo, allocationCallbacks, &memory) != VK_SUCCESS){
			spdlog::error("Failed to allocate vulkan image memory");
			throw std::runtime_error("Failed to allocate image memory");
		}

		if (vkBindImageMemory(device.get(), image, memory, 0) != VK_SUCCESS){
			spdlog::error("Failed to bind vulkan image memory");
			throw std::runtime_error("Failed to bind image memory");
		}

		imageViewInfo.image = image;

		if (vkCreateImageView(device.get(), &imageViewInfo, allocationCallbacks, &imageView) != VK_SUCCESS){
			throw std::runtime_error("Failed to create image view");
		}
	}

	Framebuffer::Framebuffer(Context& context) : Framebuffer(context, {1080, 720}){}

	Framebuffer::Framebuffer(Context& context, glm::uvec2 size) : 
		_context{context},
		_framebuffer{VK_NULL_HANDLE},
		_size{size}{

		createAttachments();
		createFramebuffer();
	}

	Framebuffer::~Framebuffer(){
		reset();
	}

	void Framebuffer::reset(){
		const auto& device = _context.renderer.device.get();
		const auto& allocationCallbacks = _context.renderer.allocationCallbacks;

		if (_framebuffer != VK_NULL_HANDLE){
			vkDestroyFramebuffer(device, _framebuffer, allocationCallbacks);
			_framebuffer = VK_NULL_HANDLE;
		}

		destroyAttachments();
	}


	void Framebuffer::resize(const glm::uvec2 size){
		#if !NDEBUG
			auto& limits = _context.renderer.physicalDevice.limits();
			assert(size.x <= limits.maxFramebufferWidth);
			assert(size.y <= limits.maxFramebufferHeight);
		#endif

		_size = size;
		reset();
		createAttachments();
		createFramebuffer();

		_context.set.update();
	}

	void Framebuffer::createFramebuffer(){
		assert(_depthAttachment && _colorAttachment && "Cannot create the framebuffer without proper attachments");
		const auto& device = _context.renderer.device.get();
		const auto& allocationCallbacks = _context.renderer.allocationCallbacks;

		VkFramebufferCreateInfo info{};
		info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;

		info.width = static_cast<uint32_t>(_size.x);
		info.height = static_cast<uint32_t>(_size.y);
		info.layers = 1;

		VkImageView attachments[] = {_depthAttachment->imageView, _colorAttachment->imageView};

		info.attachmentCount = sizeof(attachments) / sizeof(attachments[0]);
		info.pAttachments = attachments;

		info.renderPass = _context.renderPass.get();
		
		if (vkCreateFramebuffer(device, &info, allocationCallbacks, &_framebuffer) != VK_SUCCESS){
			throw std::runtime_error("Failed to create frame buffer");
		}
	}

	void Framebuffer::createAttachments(){
		createDepthAttachment();
		createColorAttachment();
	}

	void Framebuffer::destroyAttachments(){
		_depthAttachment.reset();
		_colorAttachment.reset();
	}

	VkImageCreateInfo Framebuffer::depthImageInfo(){
		VkImageCreateInfo info{};
		info.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		info.imageType = VK_IMAGE_TYPE_2D;
		info.format = VK_FORMAT_D32_SFLOAT;

		info.extent.width = static_cast<uint32_t>(_size.x);
		info.extent.height = static_cast<uint32_t>(_size.y);
		info.extent.depth = 1;
		info.arrayLayers = 1;

		info.samples = VK_SAMPLE_COUNT_1_BIT;

		info.mipLevels = 1;
		info.tiling = VK_IMAGE_TILING_OPTIMAL;
		info.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;

		info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		info.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

		return info;
	}

	VkImageViewCreateInfo Framebuffer::depthImageViewInfo(){
		VkImageViewCreateInfo info{};

		info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		// info.image : Set afterward
		info.components = {
			.r = VK_COMPONENT_SWIZZLE_R,
			.g = VK_COMPONENT_SWIZZLE_G,
			.b = VK_COMPONENT_SWIZZLE_B,
			.a = VK_COMPONENT_SWIZZLE_A
		};
		info.format = VK_FORMAT_D32_SFLOAT;
		info.viewType = VK_IMAGE_VIEW_TYPE_2D;
		info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
		info.subresourceRange.baseArrayLayer = 0;
		info.subresourceRange.baseMipLevel = 0;
		info.subresourceRange.layerCount = 1;
		info.subresourceRange.levelCount = 1;

		return info;
	}

	VkImageCreateInfo Framebuffer::colorImageInfo(){
		VkImageCreateInfo info{};
		info.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		info.imageType = VK_IMAGE_TYPE_2D;
		info.format = VK_FORMAT_R8G8B8A8_SRGB;

		info.extent.width = static_cast<uint32_t>(_size.x);
		info.extent.height = static_cast<uint32_t>(_size.y);
		info.extent.depth = 1;
		info.arrayLayers = 1;

		info.samples = VK_SAMPLE_COUNT_1_BIT;

		info.mipLevels = 1;
		info.tiling = VK_IMAGE_TILING_OPTIMAL;
		info.usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;

		info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		info.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

		return info;
	}

	VkImageViewCreateInfo Framebuffer::colorImageViewInfo(){
		VkImageViewCreateInfo info{};

		info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		// info.image : Set afterward
		info.components = {
			.r = VK_COMPONENT_SWIZZLE_R,
			.g = VK_COMPONENT_SWIZZLE_G,
			.b = VK_COMPONENT_SWIZZLE_B,
			.a = VK_COMPONENT_SWIZZLE_A
		};
		info.format = VK_FORMAT_R8G8B8A8_SRGB;
		info.viewType = VK_IMAGE_VIEW_TYPE_2D;
		info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		info.subresourceRange.baseArrayLayer = 0;
		info.subresourceRange.baseMipLevel = 0;
		info.subresourceRange.layerCount = 1;
		info.subresourceRange.levelCount = 1;

		return info;
	}

	void Framebuffer::createDepthAttachment(){
		_depthAttachment = std::make_unique<Attachment>(_context);
		_depthAttachment->create(depthImageInfo(), depthImageViewInfo());
	}

	void Framebuffer::createColorAttachment(){
		_colorAttachment = std::make_unique<Attachment>(_context);
		_colorAttachment->create(colorImageInfo(), colorImageViewInfo());
	}

	const Framebuffer::Attachment& Framebuffer::depth() const{
		return *_depthAttachment;
	}
	
	const Framebuffer::Attachment& Framebuffer::color() const{
		return *_colorAttachment;
	}

	VkFramebuffer Framebuffer::framebuffer() const{
		return _framebuffer;
	}
	
	glm::uvec2 Framebuffer::size() const{
		return _size;
	}
}