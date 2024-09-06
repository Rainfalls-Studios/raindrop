#include "Raindrop/Graphics/Vulkan/RenderPass.hpp"
#include "Raindrop/Graphics/Context.hpp"

namespace Raindrop::Graphics::Vulkan{
	std::shared_ptr<RenderPass> RenderPass::Create(const RenderPass::Builder& builder){
		return builder._context.getRegistry().emplace<RenderPass>(builder);
	}

	// ======================== ATTACHMENT DESCRIPTION

	RenderPass::AttachmentDescription::AttachmentDescription(VkAttachmentDescription& description, const std::uint32_t& index) noexcept :
		_description{&description},
		_index{index}
	{}

	RenderPass::AttachmentDescription& RenderPass::AttachmentDescription::setFlags(const VkAttachmentDescriptionFlags& flags) noexcept{
		_description->flags = flags;
		return *this;
	}

	RenderPass::AttachmentDescription& RenderPass::AttachmentDescription::setFormat(const VkFormat& format) noexcept{
		_description->format = format;
		return *this;
	}

	RenderPass::AttachmentDescription& RenderPass::AttachmentDescription::setLoadOperation(const VkAttachmentLoadOp& operation) noexcept{
		_description->loadOp = operation;
		return *this;
	}

	RenderPass::AttachmentDescription& RenderPass::AttachmentDescription::setStoreOperation(const VkAttachmentStoreOp& operation) noexcept{
		_description->storeOp = operation;
		return *this;
	}

	RenderPass::AttachmentDescription& RenderPass::AttachmentDescription::setStencilLoadOperation(const VkAttachmentLoadOp& operation) noexcept{
		_description->stencilLoadOp = operation;
		return *this;
	}

	RenderPass::AttachmentDescription& RenderPass::AttachmentDescription::setStencilStoreOperation(const VkAttachmentStoreOp& operation) noexcept{
		_description->stencilStoreOp = operation;
		return *this;
	}

	RenderPass::AttachmentDescription& RenderPass::AttachmentDescription::setInitialLayout(const VkImageLayout& layout) noexcept{
		_description->initialLayout = layout;
		return *this;
	}

	RenderPass::AttachmentDescription& RenderPass::AttachmentDescription::setFinalLayout(const VkImageLayout& layout) noexcept{
		_description->finalLayout = layout;
		return *this;
	}


	VkAttachmentDescription& RenderPass::AttachmentDescription::get() noexcept{
		return *_description;
	}

	const std::uint32_t& RenderPass::AttachmentDescription::getIndex() const noexcept{
		return _index;
	}


	// ==================================== SUBPASS DESCRIPTION

	RenderPass::SubpassDescription RenderPass::SubpassDescription::External;

	RenderPass::SubpassDescription::SubpassDescription() : 
		_description{nullptr},
		_data{nullptr},
		_index{0}
	{}

	RenderPass::SubpassDescription::SubpassDescription(VkSubpassDescription& description, BuildInfo::SubpassData& data, const std::uint32_t& index) noexcept :
		_description{&description},
		_data{&data},
		_index{index}
	{}
					
	RenderPass::SubpassDescription& RenderPass::SubpassDescription::setFlags(const VkSubpassDescriptionFlags& flags) noexcept{
		_description->flags = flags;
		return *this;
	}

	RenderPass::SubpassDescription& RenderPass::SubpassDescription::setDepthAttachment(const AttachmentDescription& attachment, VkImageLayout layout){
		_data->depth = VkAttachmentReference{
			.attachment = attachment.getIndex(),
			.layout = layout
		};

		_description->pDepthStencilAttachment = &_data->depth;
		return *this;
	}

	RenderPass::SubpassDescription& RenderPass::SubpassDescription::addColorAttachment(const AttachmentDescription& attachment, VkImageLayout layout){
		_data->colorAttachments.push_back(VkAttachmentReference{
			.attachment = attachment.getIndex(),
			.layout = layout
		});

		_description->pColorAttachments = _data->colorAttachments.data();
		_description->colorAttachmentCount = static_cast<uint32_t>(_data->colorAttachments.size());

		return *this;
	}

	RenderPass::SubpassDescription& RenderPass::SubpassDescription::addInputAttachment(const AttachmentDescription& attachment, VkImageLayout layout){
		_data->inputAttachments.push_back(VkAttachmentReference{
			.attachment = attachment.getIndex(),
			.layout = layout
		});

		_description->pInputAttachments = _data->inputAttachments.data();
		_description->inputAttachmentCount = static_cast<uint32_t>(_data->inputAttachments.size());

		return *this;
	}

	RenderPass::SubpassDescription& RenderPass::SubpassDescription::addPreserveAttachment(const AttachmentDescription& attachment){
		_data->preserveAttachments.push_back(attachment.getIndex());

		_description->pPreserveAttachments = _data->preserveAttachments.data();
		_description->preserveAttachmentCount = static_cast<uint32_t>(_data->preserveAttachments.size());

		return *this;
	}

	VkSubpassDescription& RenderPass::SubpassDescription::get() noexcept{
		return *_description;
	}

	const std::uint32_t& RenderPass::SubpassDescription::getIndex() const noexcept{
		return _index;
	}
	
	// ==================================== SUBPASS DEPENDENCY

	RenderPass::Dependency::Dependency(VkSubpassDependency& dependency) noexcept: 
		_dependency{&dependency}
	{}

	RenderPass::Dependency& RenderPass::Dependency::setFlags(const VkDependencyFlags& flags) noexcept{
		_dependency->dependencyFlags = flags;
		return *this;
	}

	RenderPass::Dependency& RenderPass::Dependency::setSrcSubpass(const SubpassDescription& subpass) noexcept{
		_dependency->srcSubpass = subpass.getIndex();
		return *this;
	}

	RenderPass::Dependency& RenderPass::Dependency::setDstSubpass(const SubpassDescription& subpass) noexcept{
		_dependency->dstAccessMask = subpass.getIndex();
		return *this;
	}

	RenderPass::Dependency& RenderPass::Dependency::setSrcStage(const VkPipelineStageFlags& stage) noexcept{
		_dependency->srcStageMask = stage;
		return *this;
	}

	RenderPass::Dependency& RenderPass::Dependency::setDstStage(const VkPipelineStageFlags& stage) noexcept{
		_dependency->dstStageMask = stage;
		return *this;
	}

	RenderPass::Dependency& RenderPass::Dependency::setSrcAccess(const VkAccessFlags& access) noexcept{
		_dependency->srcAccessMask = access;
		return *this;
	}

	RenderPass::Dependency& RenderPass::Dependency::setDstAccess(const VkAccessFlags& access) noexcept{
		_dependency->dstAccessMask = access;
		return *this;
	}

	VkSubpassDependency& RenderPass::Dependency::get() noexcept{
		return *_dependency;
	}

	// ========================== RENDER PASS

	RenderPass::RenderPass(const Builder& builder) : 
		_context{builder._context},
		_renderPass{VK_NULL_HANDLE}
	{

		auto& device = _context.getDevice();

		VkRenderPassCreateInfo info = builder._info.generateInfo();
		if (vkCreateRenderPass(device.get(), &info, nullptr, &_renderPass) != VK_SUCCESS){
			SPDLOG_LOGGER_ERROR(_context.getLogger(), "Failed to create render pass");
			throw std::runtime_error("Failed to create render pass");
		}
	}

	RenderPass::~RenderPass(){
		if (_renderPass != VK_NULL_HANDLE){
			SPDLOG_LOGGER_INFO(_context.getLogger(), "Destroying render pass...");

			vkDestroyRenderPass(_context.getDevice().get(), _renderPass, nullptr);
			_renderPass = VK_NULL_HANDLE;
		}
	}

	const VkRenderPass& RenderPass::get() const noexcept{
		return _renderPass;
	}

	void RenderPass::Builder::setCreateFlags(const VkRenderPassCreateFlags& flags){
		_info.flags = flags;
	}

	RenderPass::AttachmentDescription RenderPass::Builder::addAttachment(){
		static constexpr VkAttachmentDescription DEFAULT{
			.flags = 0,
			.format = VK_FORMAT_UNDEFINED,
			.samples = VK_SAMPLE_COUNT_1_BIT,
			.loadOp = VK_ATTACHMENT_LOAD_OP_LOAD,
			.storeOp = VK_ATTACHMENT_STORE_OP_STORE,
			.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_LOAD,
			.stencilStoreOp = VK_ATTACHMENT_STORE_OP_STORE,
			.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
			.finalLayout = VK_IMAGE_LAYOUT_UNDEFINED
		};

		std::uint32_t index = static_cast<std::uint32_t>(_info.attachmentDescriptions.size());
		VkAttachmentDescription& description = _info.attachmentDescriptions.emplace_back(DEFAULT);

		return AttachmentDescription(description, index);
	}

	RenderPass::SubpassDescription RenderPass::Builder::addSubpass(){
		static constexpr VkSubpassDescription DEFAULT{
			.flags = 0,
			.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS,
			.inputAttachmentCount = 0,
			.pInputAttachments = nullptr,
			.colorAttachmentCount = 0,
			.pColorAttachments = nullptr,
			.pResolveAttachments = nullptr,
			.pDepthStencilAttachment = nullptr,
			.preserveAttachmentCount = 0,
			.pPreserveAttachments = nullptr
		};
		
		std::uint32_t index = static_cast<std::uint32_t>(_info.subpassDescriptions.size());
		VkSubpassDescription& description = _info.subpassDescriptions.emplace_back(DEFAULT);
		BuildInfo::SubpassData& data = _info.subpassData.emplace_back();

		return SubpassDescription(description, data, index);
	}

	RenderPass::Dependency RenderPass::Builder::addDependency(){
		static constexpr VkSubpassDependency DEFAULT{
			.srcSubpass = VK_SUBPASS_EXTERNAL,
			.dstSubpass = VK_SUBPASS_EXTERNAL,
			.srcStageMask = 0,
			.dstStageMask = 0,
			.srcAccessMask = 0,
			.dstAccessMask = 0,
			.dependencyFlags = 0
		};

		VkSubpassDependency& dependency = _info.subpassDependencies.emplace_back(DEFAULT);

		return Dependency(dependency);
	}

	VkRenderPassCreateInfo RenderPass::BuildInfo::generateInfo() const{
		return VkRenderPassCreateInfo{
			.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
			.pNext = nullptr,
			.flags = flags,
			.attachmentCount = static_cast<uint32_t>(attachmentDescriptions.size()),
			.pAttachments = attachmentDescriptions.data(),
			.subpassCount = static_cast<uint32_t>(subpassDescriptions.size()),
			.pSubpasses = subpassDescriptions.data(),
			.dependencyCount = static_cast<uint32_t>(subpassDependencies.size()),
			.pDependencies = subpassDependencies.data()
		};
	}

	RenderPass::Builder::Builder(Context& context) : _context{context}{}
	RenderPass::Builder::~Builder() {}
}