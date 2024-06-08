#include <Raindrop/RenderPass.hpp>
#include <Raindrop/GUID.hpp>
#include <Raindrop_internal/RenderPass.hpp>
#include <Raindrop_internal/Graphics/Context.hpp>
#include <Raindrop_internal/Context.hpp>
#include <Raindrop_internal/Graphics/Renderer.hpp>
#include <Raindrop_internal/Texture.hpp>
#include <Raindrop_internal/Format.hpp>
#include <Raindrop_internal/Pipeline.hpp>


#define LOGGER _impl->context->getInternalContext()->getLogger()
#define GRAPHICS_CONTEXT _impl->context->getInternalContext()->getRenderer().getContext()
#define ATTACHMENT_DATA reinterpret_cast<VkAttachmentDescription*>(_data)
#define SUBPASS_DATA reinterpret_cast<Raindrop::Internal::Graphics::RenderPassConfigInfo::SubpassInfo*>(_data)
#define DEPENDENCY_DATA reinterpret_cast<VkSubpassDependency*>(_data)

namespace Raindrop{
	RenderPass::Attachment::Attachment() noexcept : 
		_flags{Flags::NONE},
		_format{Format::UNDEFINED},
		_loadOperation{Operation::LOAD},
		_storeOperation{Operation::STORE},
		_stencilLoadOperation{Operation::DONT_LOAD},
		_stencilStoreOperation{Operation::DONT_STORE},
		_initialLayout{Texture::Layout::UNDEFINED},
		_finalLayout{Texture::Layout::UNDEFINED}
	{}

	RenderPass::Attachment& RenderPass::Attachment::setFlags(const Flags& flags) noexcept{
		_flags = flags;
		return *this;
	}

	RenderPass::Attachment& RenderPass::Attachment::setFormat(const Format& format) noexcept{
		_format = format;
		return *this;
	}

	RenderPass::Attachment& RenderPass::Attachment::setLoadOperation(const Operation::Load& operation) noexcept{
		_loadOperation = operation;
		return *this;
	}

	RenderPass::Attachment& RenderPass::Attachment::setStoreOperation(const Operation::Store& operation) noexcept{
		_storeOperation = operation;
		return *this;
	}

	RenderPass::Attachment& RenderPass::Attachment::setStencilLoadOperation(const Operation::Load& operation) noexcept{
		_stencilLoadOperation = operation;
		return *this;
	}

	RenderPass::Attachment& RenderPass::Attachment::setStencilStoreOperation(const Operation::Store& operation) noexcept{
		_stencilStoreOperation = operation;
		return *this;
	}

	RenderPass::Attachment& RenderPass::Attachment::setInitialLayout(const Texture::Layout& layout) noexcept{
		_initialLayout = layout;
		return *this;
	}

	RenderPass::Attachment& RenderPass::Attachment::setFinalLayout(const Texture::Layout& layout) noexcept{
		_finalLayout = layout;
		return *this;
	}

	const RenderPass::Attachment::Flags& RenderPass::Attachment::getFlags() const noexcept{
		return _flags;
	}

	const Format& RenderPass::Attachment::getFormat() const noexcept{
		return _format;
	}

	const RenderPass::Attachment::Operation::Load& RenderPass::Attachment::getLoadOperation() const noexcept{
		return _loadOperation;
	}

	const RenderPass::Attachment::Operation::Store& RenderPass::Attachment::getStoreOperation() const noexcept{
		return _storeOperation;
	}

	const RenderPass::Attachment::Operation::Load& RenderPass::Attachment::getStencilLoadOperation() const noexcept{
		return _stencilLoadOperation;
	}

	const RenderPass::Attachment::Operation::Store& RenderPass::Attachment::getStencilStoreOperation() const noexcept{
		return _stencilStoreOperation;
	}

	const Texture::Layout& RenderPass::Attachment::getInitialLayout() const noexcept{
		return _initialLayout;
	}

	const Texture::Layout& RenderPass::Attachment::getFinalLayout() const noexcept{
		return _finalLayout;
	}

	//--------------------------------------------------------------------
	//-----------------            Subpass               -----------------
	//--------------------------------------------------------------------

	static constexpr RenderPass::Subpass::Flags::Bits SUBPASS_EXTERNAL_FLAG_BIT = RenderPass::Subpass::Flags::Bits(1 << 31);

	const RenderPass::Subpass RenderPass::Subpass::External = RenderPass::Subpass().setFlags(SUBPASS_EXTERNAL_FLAG_BIT);


	RenderPass::Subpass::Subpass() : 
		_flags{Flags::NONE},
		_depthAttachment{},
		_colorAttachments{},
		_inputAttachments{},
		_preserveAttachments{}
	{}

	RenderPass::Subpass& RenderPass::Subpass::setFlags(const Flags& flags) noexcept{
		_flags = flags;
		return *this;
	}

	RenderPass::Subpass& RenderPass::Subpass::setDepthAttachment(const AttachmentReference& attachment){
		_depthAttachment.emplace(attachment);
		return *this;
	}

	RenderPass::Subpass& RenderPass::Subpass::addColorAttachment(const AttachmentReference& attachment){
		_colorAttachments.emplace_back(attachment);
		return *this;
	}

	RenderPass::Subpass& RenderPass::Subpass::addInputAttachment(const AttachmentReference& attachment){
		_inputAttachments.emplace_back(attachment);
		return *this;
	}

	RenderPass::Subpass& RenderPass::Subpass::addPreserveAttachment(const Attachment& attachment){
		_preserveAttachments.emplace_back(std::cref(attachment));
		return *this;
	}

	const RenderPass::Subpass::Flags& RenderPass::Subpass::getFlags() const noexcept{
		return _flags;
	}

	const std::optional<RenderPass::Subpass::AttachmentReference>& RenderPass::Subpass::getDepthAttachment() const noexcept{
		return _depthAttachment;
	}

	const std::list<RenderPass::Subpass::AttachmentReference>& RenderPass::Subpass::getColorAttachments() const noexcept{
		return _colorAttachments;
	}

	const std::list<RenderPass::Subpass::AttachmentReference>& RenderPass::Subpass::getInputAttachments() const noexcept{
		return _inputAttachments;
	}

	const std::list<std::reference_wrapper<const RenderPass::Attachment>>& RenderPass::Subpass::getPreserveAttachments() const noexcept{
		return _preserveAttachments;
	}


	//--------------------------------------------------------------------
	//-----------------           Dependency             -----------------
	//--------------------------------------------------------------------

	RenderPass::Dependency::Dependency() : 
		_flags{RenderPass::Dependency::Flags::NONE},
		_srcSubpass{&RenderPass::Subpass::External},
		_dstSubpass{&RenderPass::Subpass::External},
		_srcStage{Pipeline::Stage::NONE},
		_dstStage{Pipeline::Stage::NONE},
		_srcAccess{RenderPass::Access::NONE},
		_dstAccess{RenderPass::Access::NONE}
	{}

	RenderPass::Dependency& RenderPass::Dependency::setFlags(const Flags& flags){
		_flags = flags;
		return *this;
	}

	RenderPass::Dependency& RenderPass::Dependency::setSrcSubpass(const Subpass& subpass){
		_srcSubpass = &subpass;
		return *this;
	}

	RenderPass::Dependency& RenderPass::Dependency::setDstSubpass(const Subpass& subpass){
		_dstSubpass = &subpass;
		return *this;
	}

	RenderPass::Dependency& RenderPass::Dependency::setSrcStage(const Pipeline::Stage& stage){
		_srcStage = stage;
		return *this;
	}

	RenderPass::Dependency& RenderPass::Dependency::setDstStage(const Pipeline::Stage& stage){
		_dstStage = stage;
		return *this;
	}

	RenderPass::Dependency& RenderPass::Dependency::setSrcAccess(const Access& access){
		_srcAccess = access;
		return *this;
	}

	RenderPass::Dependency& RenderPass::Dependency::setDstAccess(const Access& access){
		_dstAccess = access;
		return *this;
	}

	const RenderPass::Dependency::Flags& RenderPass::Dependency::getFlags() const noexcept{
		return _flags;
	}

	const RenderPass::Subpass& RenderPass::Dependency::getSrcSubpass() const noexcept{
		return *_srcSubpass;
	}

	const RenderPass::Subpass& RenderPass::Dependency::getDstSubpass() const noexcept{
		return *_dstSubpass;
	}

	const Pipeline::Stage& RenderPass::Dependency::getSrcStage() const noexcept{
		return _srcStage;
	}

	const Pipeline::Stage& RenderPass::Dependency::getDstStage() const noexcept{
		return _dstStage;
	}

	const RenderPass::Access& RenderPass::Dependency::getSrcAccess() const noexcept{
		return _srcAccess;
	}

	const RenderPass::Access& RenderPass::Dependency::getDstAccess() const noexcept{
		return _dstAccess;
	}

	//--------------------------------------------------------------------
	//-----------------          Render pass             -----------------
	//--------------------------------------------------------------------

	RenderPass::Impl::Builder::Builder() : 
		info{
			.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
			.pNext = nullptr,
			.flags = 0,
			.attachmentCount = 0,
			.pAttachments = nullptr,
			.subpassCount = 0,
			.pSubpasses = nullptr,
			.dependencyCount = 0,
			.pDependencies = nullptr
		},
		attachments{},
		subpasses{},
		dependencies{}
	{}

	void RenderPass::Impl::Builder::update(){

		translation.references.clear();
		translation.preserveAttachments.clear();
		translation.depthStencils.clear();
		
		// ATTACHMENTS
		translation.attachments.resize(attachments.size());

		for (std::size_t i=0; i<attachments.size(); i++){
			const auto& src = attachments[i];

			translation.attachments[i] = VkAttachmentDescription{
				.flags = AttachmentFlagsToVulkan(src.getFlags()),
				.format = FormatToVulkan(src.getFormat()),
				.samples = VK_SAMPLE_COUNT_1_BIT,
				.loadOp = AttachmentLoadOperationToVulkan(src.getLoadOperation()),
				.storeOp = AttachmentStoreOperationToVulkan(src.getStoreOperation()),
				.stencilLoadOp = AttachmentLoadOperationToVulkan(src.getStencilLoadOperation()),
				.stencilStoreOp = AttachmentStoreOperationToVulkan(src.getStencilStoreOperation()),
				.initialLayout = TextureLayoutToVulkan(src.getInitialLayout()),
				.finalLayout = TextureLayoutToVulkan(src.getFinalLayout())
			};
		}
		
		// SUBPASSES
		translation.subpasses.resize(subpasses.size());

		for (std::size_t i=0; i<subpasses.size(); i++){
			const auto& src = subpasses[i];

			uint32_t inputAttachmentCount = 0;
			{
				const auto& input = src.getInputAttachments();
				inputAttachmentCount = static_cast<uint32_t>(input.size());

				for (const auto& attachment : input){
					VkImageLayout layout = TextureLayoutToVulkan(attachment.layout);
					uint32_t index = &attachment.attachment - &attachments[0];

					translation.references.emplace_back(
						VkAttachmentReference{
							.attachment = index,
							.layout = layout
						}
					);
				}
			}
			VkAttachmentReference* inputAttachments = translation.references.data() + (translation.references.size() - inputAttachmentCount);

			uint32_t colorAttachmentCount = 0;
			{
				const auto& input = src.getColorAttachments();
				colorAttachmentCount = static_cast<uint32_t>(input.size());

				for (const auto& attachment : input){
					VkImageLayout layout = TextureLayoutToVulkan(attachment.layout);
					uint32_t index = &attachment.attachment - &attachments[0];

					translation.references.emplace_back(
						VkAttachmentReference{
							.attachment = index,
							.layout = layout
						}
					);
				}
			}
			VkAttachmentReference* colorAttachments = translation.references.data() + (translation.references.size() - colorAttachmentCount);

			uint32_t preserveAttachmentCount = 0;
			{
				const auto& input = src.getPreserveAttachments();
				preserveAttachmentCount = static_cast<uint32_t>(input.size());

				for (const auto& attachment : input){
					uint32_t index = &attachment.get() - &attachments[0];
					translation.preserveAttachments.push_back(index);
				}
			}
			uint32_t* preserveAttachments = translation.preserveAttachments.data() + (translation.preserveAttachments.size() - preserveAttachmentCount);

			VkAttachmentReference* depthStencil = nullptr;

			if (src.getDepthAttachment().has_value()){
				auto& attachment = src.getDepthAttachment();

				translation.depthStencils.emplace_back(
					VkAttachmentReference{
						.attachment = uint32_t(&attachment->attachment - &attachments[0]),
						.layout = TextureLayoutToVulkan(attachment->layout)
					}
				);

				depthStencil = &translation.depthStencils.back();
			}

			translation.subpasses[i] = VkSubpassDescription{
				.flags = SubpassFlagsToVulkan(src.getFlags()),
				.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS,
				.inputAttachmentCount = inputAttachmentCount,
				.pInputAttachments = inputAttachments,
				.colorAttachmentCount = colorAttachmentCount,
				.pColorAttachments = colorAttachments,
				.pResolveAttachments = nullptr,
				.pDepthStencilAttachment = depthStencil,
				.preserveAttachmentCount = preserveAttachmentCount,
				.pPreserveAttachments = preserveAttachments
			};
		}

		// DEPENDENCIES
		translation.dependencies.resize(dependencies.size());

		for (std::size_t i=0; i<dependencies.size(); i++){
			const auto& src = dependencies[i];

			uint32_t srcSubpass = &src.getSrcSubpass() == &Subpass::External ? VK_SUBPASS_EXTERNAL : static_cast<uint32_t>(&src.getSrcSubpass() - &subpasses[0]);
			uint32_t dstSubpass = &src.getDstSubpass() == &Subpass::External ? VK_SUBPASS_EXTERNAL : static_cast<uint32_t>(&src.getDstSubpass() - &subpasses[0]);

			translation.dependencies[i] = VkSubpassDependency{
				.srcSubpass = srcSubpass,
				.dstSubpass = dstSubpass,
				.srcStageMask = toVulkan(src.getSrcStage()),
				.dstStageMask = toVulkan(src.getDstStage()),
				.srcAccessMask = AccessFlagsToVulkan(src.getSrcAccess()),
				.dstAccessMask = AccessFlagsToVulkan(src.getDstAccess()),
				.dependencyFlags = SubpassDependencyFlagsToVulkan(src.getFlags())
			};
		}
		
		info.attachmentCount = static_cast<uint32_t>(translation.attachments.size());
		info.pAttachments = translation.attachments.data();

		info.subpassCount = static_cast<uint32_t>(translation.subpasses.size());
		info.pSubpasses = translation.subpasses.data();

		info.dependencyCount = static_cast<uint32_t>(translation.dependencies.size());
		info.pDependencies = translation.dependencies.data();
	}

	RenderPass RenderPass::Create(Context& context){
		return RenderPass(context);
	}

	RenderPass::RenderPass(Context& context) : _impl{std::make_unique<Impl>()}{
		_impl->builder = std::make_unique<Impl::Builder>();
		_impl->context = &context;
	}

	RenderPass::~RenderPass(){
		_impl.reset();
	}

	void RenderPass::initialize(){
		if (!_impl->builder){
			LOGGER->warn("Cannot initialize an already initialized render pass");
			return;
		}

		LOGGER->info("Initializing render pass ...");

		_impl->builder->update();

		_impl->renderPass = std::make_shared<Internal::Graphics::RenderPass>(GRAPHICS_CONTEXT, _impl->builder->info);
		_impl->builder.reset();

		LOGGER->info("Render pass initialized with success !");

	}

	void RenderPass::release(){
		_impl->renderPass.reset();
		_impl->builder = std::make_unique<Impl::Builder>();
	}

	bool RenderPass::isInitialized() const noexcept{
		return _impl->renderPass != nullptr;
	}

	void* RenderPass::getNativeHandle() const noexcept{
		return static_cast<void*>(_impl->renderPass->get());
	}

	RenderPass::Impl* RenderPass::getImpl() const noexcept{
		return _impl.get();
	}

	GUID RenderPass::getGUID() const noexcept{

	}

	RenderPass::Attachment& RenderPass::addAttachment(){
		return _impl->builder->attachments.emplace_back();
	}

	RenderPass::Subpass& RenderPass::addSubpass(){
		return _impl->builder->subpasses.emplace_back();
	}

	RenderPass::Dependency& RenderPass::addDependency(){
		return _impl->builder->dependencies.emplace_back();
	}
}