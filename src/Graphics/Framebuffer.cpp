#include <Raindrop/Graphics/Framebuffer.hpp>
#include <Raindrop/Graphics/Context.hpp>
#include <Raindrop/Graphics/ImageView.hpp>
#include <Raindrop/Graphics/RenderPass.hpp>
#include <Raindrop/Exceptions/VulkanExceptions.hpp>
#include <Raindrop/Context.hpp>

namespace Raindrop::Graphics{
	std::shared_ptr<Framebuffer> Framebuffer::create(Raindrop::Context& context){
		return context.registry.emplace<Framebuffer>();
	}

	Framebuffer::Framebuffer() noexcept : 
		_context{nullptr},
		_framebuffer{VK_NULL_HANDLE},
		_renderPass{},
		_attachments{},
		_info{}
	{}

	Framebuffer::~Framebuffer(){
		release();
	}

	Framebuffer& Framebuffer::prepare(Context& context){
		_context = &context;
		_info = std::make_unique<BuildInfo>();

		return *this;
	}

	void Framebuffer::initialize(){
		if (!_context){
			spdlog::error("Attempt to initialize a non prepared frame buffer");
			throw std::runtime_error("Attempt to initialize a non prepared frame buffer");
		}

		std::vector<VkImageView> vkAttachments;
		for (auto& attachment : _info->attachments){
			vkAttachments.push_back(attachment->get());
		}

		_renderPass = _info->renderPass;
		_attachments = _info->attachments;

		VkFramebufferCreateInfo info{
			.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
			.pNext = nullptr,
			.flags = _info->flags,
			.renderPass = _renderPass->get(),
			.attachmentCount = static_cast<uint32_t>(vkAttachments.size()),
			.pAttachments = vkAttachments.data(),
			.width = _info->width,
			.height = _info->height,
			.layers = _info->layers
		};

		auto& device = _context->getDevice();
		auto& allocationCallbacks = _context->core.allocationCallbacks;

		Exceptions::checkVkCreation<VkFramebuffer>(
			vkCreateFramebuffer(device.get(), &info, allocationCallbacks, &_framebuffer),
			"Failed to create frame buffer",
			_context->logger
		);

		_info.reset();
	}

	void Framebuffer::release(){
		if (!_context) return;

		auto& device = _context->getDevice();
		auto& allocationCallbacks = _context->core.allocationCallbacks;

		_renderPass.reset();
		_attachments.clear();

		if (!_framebuffer){
			vkDestroyFramebuffer(device.get(), _framebuffer, allocationCallbacks);
			_framebuffer = VK_NULL_HANDLE;
		}

		_context = nullptr;
		_info.reset();
	}

	const VkFramebuffer& Framebuffer::get() const noexcept{
		return _framebuffer;
	}

	Framebuffer::BuildInfo& Framebuffer::getInfo(){
		if (!_context){
			spdlog::warn("Attempt to access build info of a non prepared frame buffer");
			throw std::runtime_error("The frame buffer has not been prepared");
		}

		if (!_info){
			_context->logger->warn("Attempt to access build info of an already built frame buffer");
			throw std::runtime_error("The frame buffer has already been initialized");
		}

		return *_info;
	}

	Framebuffer& Framebuffer::setFlags(const VkFramebufferCreateFlags& flags){
		getInfo().flags = flags;
		return *this;
	}

	Framebuffer& Framebuffer::setWidth(const std::uint32_t& width){
		getInfo().width = width;
		return *this;
	}

	Framebuffer& Framebuffer::setHeight(const std::uint32_t& height){
		getInfo().height = height;
		return *this;
	}

	Framebuffer& Framebuffer::setLayers(const std::uint32_t& layers){
		getInfo().layers = layers;
		return *this;
	}

	Framebuffer& Framebuffer::setSize(const std::uint32_t& width, const std::uint32_t& height, const std::uint32_t& layers){
		setWidth(width);
		setHeight(height);
		setLayers(layers);
		return *this;
	}

	Framebuffer& Framebuffer::setRenderPass(const std::shared_ptr<RenderPass>& renderPass){
		getInfo().renderPass = renderPass;
		return *this;
	}

	Framebuffer& Framebuffer::setAttachments(const std::list<std::shared_ptr<ImageView>>& attachments){
		getInfo().attachments = attachments;
		return *this;
	}
}