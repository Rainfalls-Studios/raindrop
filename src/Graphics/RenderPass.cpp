#include <Raindrop/Graphics/RenderPass.hpp>
#include <Raindrop/Graphics/Context.hpp>
#include <Raindrop/Graphics/Core/Context.hpp>
#include <Raindrop/Exceptions/VulkanExceptions.hpp>
#include <Raindrop/Context.hpp>

namespace Raindrop::Graphics{
	std::shared_ptr<RenderPass> RenderPass::create(Raindrop::Context& context){
		return context.registry.emplace<RenderPass>();
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

	RenderPass::RenderPass() noexcept : 
		_context{nullptr},
		_renderPass{VK_NULL_HANDLE}
	{}

	RenderPass::~RenderPass(){
		release();
	}

	RenderPass::RenderPass(RenderPass&& other) : 
		_context{nullptr},
		_renderPass{VK_NULL_HANDLE}{
		swap(*this, other);
	}

	RenderPass& RenderPass::operator=(RenderPass&& other){
		swap(*this, other);
		return *this;
	}

	void swap(RenderPass& A, RenderPass& B){
		std::swap(A._context, B._context);
		std::swap(A._renderPass, B._renderPass);
		std::swap(A._buildInfo, B._buildInfo);
	}

	void RenderPass::prepare(Context& context){
		_context = &context;

		_buildInfo = std::make_unique<BuildInfo>();
	}

	void RenderPass::initialize(){
		_context->logger->info("Creating render pass...");

		VkRenderPassCreateInfo info = _buildInfo->generateInfo();

		Exceptions::checkVkCreation<VkRenderPass>(
			vkCreateRenderPass(_context->core.device.get(), &info, _context->core.allocationCallbacks, &_renderPass),
			"Failed to create render pass",
			_context->logger
		);

		_buildInfo.reset();
	}

	void RenderPass::release(){
		_buildInfo.reset();
		
		if (_renderPass != VK_NULL_HANDLE){
			_context->logger->info("Destroying render pass ...");

			vkDestroyRenderPass(_context->core.device.get(), _renderPass, _context->core.allocationCallbacks);
			_renderPass = VK_NULL_HANDLE;
		}

		_context = nullptr;
	}

	const VkRenderPass& RenderPass::get() const noexcept{
		return _renderPass;
	}

	void RenderPass::setCreateFlags(const VkRenderPassCreateFlags& flags){
		if (!_buildInfo) throw std::runtime_error("The render pass has already been initialized");
		_buildInfo->flags = flags;
	}

	RenderPass::AttachmentDescription RenderPass::addAttachment(){
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

		if (!_buildInfo) throw std::runtime_error("The render pass has already been initialized");

		std::uint32_t index = static_cast<std::uint32_t>(_buildInfo->attachmentDescriptions.size());
		VkAttachmentDescription& description = _buildInfo->attachmentDescriptions.emplace_back(DEFAULT);

		return AttachmentDescription(description, index);
	}

	RenderPass::SubpassDescription RenderPass::addSubpass(){
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

		if (!_buildInfo) throw std::runtime_error("The render pass has already been initialized");
		
		std::uint32_t index = static_cast<std::uint32_t>(_buildInfo->subpassDescriptions.size());
		VkSubpassDescription& description = _buildInfo->subpassDescriptions.emplace_back(DEFAULT);
		BuildInfo::SubpassData& data = _buildInfo->subpassData.emplace_back();

		return SubpassDescription(description, data, index);
	}

	RenderPass::Dependency RenderPass::addDependency(){
		static constexpr VkSubpassDependency DEFAULT{
			.srcSubpass = VK_SUBPASS_EXTERNAL,
			.dstSubpass = VK_SUBPASS_EXTERNAL,
			.srcStageMask = 0,
			.dstStageMask = 0,
			.srcAccessMask = 0,
			.dstAccessMask = 0,
			.dependencyFlags = 0
		};

		if (!_buildInfo) throw std::runtime_error("The render pass has already been initialized");

		VkSubpassDependency& dependency = _buildInfo->subpassDependencies.emplace_back(DEFAULT);

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
}