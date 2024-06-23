#include <Raindrop/RenderPass.hpp>
#include <Raindrop/GUID.hpp>
#include <Raindrop_internal/RenderPass.hpp>
#include <Raindrop_internal/Graphics/Context.hpp>
#include <Raindrop_internal/Context.hpp>
#include <Raindrop_internal/Graphics/Engine.hpp>
#include <Raindrop_internal/Texture.hpp>
#include <Raindrop_internal/Format.hpp>
#include <Raindrop_internal/Pipeline.hpp>
#include <Raindrop_internal/CommandBuffer.hpp>

#define LOGGER _impl->context->getLogger()
#define GRAPHICS_CONTEXT _impl->context->getEngine().getContext()
#define ATTACHMENT_DATA reinterpret_cast<VkAttachmentDescription*>(_data)
#define SUBPASS_DATA reinterpret_cast<Raindrop::Internal::Graphics::RenderPassConfigInfo::SubpassInfo*>(_data)
#define DEPENDENCY_DATA reinterpret_cast<VkSubpassDependency*>(_data)

#include <iostream>

namespace Raindrop{
	RenderPass::AttachmentDescription::AttachmentDescription() noexcept : 
		_flags{Flags::NONE},
		_format{Format::UNDEFINED},
		_loadOperation{Operation::LOAD},
		_storeOperation{Operation::STORE},
		_stencilLoadOperation{Operation::DONT_LOAD},
		_stencilStoreOperation{Operation::DONT_STORE},
		_initialLayout{Texture::Layout::UNDEFINED},
		_finalLayout{Texture::Layout::UNDEFINED}
	{}

	RenderPass::AttachmentDescription& RenderPass::AttachmentDescription::setFlags(const Flags& flags) noexcept{
		_flags = flags;
		return *this;
	}

	RenderPass::AttachmentDescription& RenderPass::AttachmentDescription::setFormat(const Format& format) noexcept{
		_format = format;
		return *this;
	}

	RenderPass::AttachmentDescription& RenderPass::AttachmentDescription::setLoadOperation(const Operation::Load& operation) noexcept{
		_loadOperation = operation;
		return *this;
	}

	RenderPass::AttachmentDescription& RenderPass::AttachmentDescription::setStoreOperation(const Operation::Store& operation) noexcept{
		_storeOperation = operation;
		return *this;
	}

	RenderPass::AttachmentDescription& RenderPass::AttachmentDescription::setStencilLoadOperation(const Operation::Load& operation) noexcept{
		_stencilLoadOperation = operation;
		return *this;
	}

	RenderPass::AttachmentDescription& RenderPass::AttachmentDescription::setStencilStoreOperation(const Operation::Store& operation) noexcept{
		_stencilStoreOperation = operation;
		return *this;
	}

	RenderPass::AttachmentDescription& RenderPass::AttachmentDescription::setInitialLayout(const Texture::Layout& layout) noexcept{
		_initialLayout = layout;
		return *this;
	}

	RenderPass::AttachmentDescription& RenderPass::AttachmentDescription::setFinalLayout(const Texture::Layout& layout) noexcept{
		_finalLayout = layout;
		return *this;
	}

	const RenderPass::AttachmentDescription::Flags& RenderPass::AttachmentDescription::getFlags() const noexcept{
		return _flags;
	}

	const Format& RenderPass::AttachmentDescription::getFormat() const noexcept{
		return _format;
	}

	const RenderPass::AttachmentDescription::Operation::Load& RenderPass::AttachmentDescription::getLoadOperation() const noexcept{
		return _loadOperation;
	}

	const RenderPass::AttachmentDescription::Operation::Store& RenderPass::AttachmentDescription::getStoreOperation() const noexcept{
		return _storeOperation;
	}

	const RenderPass::AttachmentDescription::Operation::Load& RenderPass::AttachmentDescription::getStencilLoadOperation() const noexcept{
		return _stencilLoadOperation;
	}

	const RenderPass::AttachmentDescription::Operation::Store& RenderPass::AttachmentDescription::getStencilStoreOperation() const noexcept{
		return _stencilStoreOperation;
	}

	const Texture::Layout& RenderPass::AttachmentDescription::getInitialLayout() const noexcept{
		return _initialLayout;
	}

	const Texture::Layout& RenderPass::AttachmentDescription::getFinalLayout() const noexcept{
		return _finalLayout;
	}

	//--------------------------------------------------------------------
	//-----------------            Subpass               -----------------
	//--------------------------------------------------------------------

	static constexpr RenderPass::Subpass::Flags::Bits SUBPASS_EXTERNAL_FLAG_BIT = RenderPass::Subpass::Flags::Bits(1 << 31);

	const RenderPass::Subpass RenderPass::Subpass::External = std::move(RenderPass::Subpass().setFlags(SUBPASS_EXTERNAL_FLAG_BIT));


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

	RenderPass::Subpass& RenderPass::Subpass::addPreserveAttachment(const AttachmentDescription& attachment){
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

	const std::list<std::reference_wrapper<const RenderPass::AttachmentDescription>>& RenderPass::Subpass::getPreserveAttachments() const noexcept{
		return _preserveAttachments;
	}


	//--------------------------------------------------------------------
	//-----------------           Dependency             -----------------
	//--------------------------------------------------------------------

	RenderPass::Dependency::Dependency() : 
		_flags{RenderPass::Dependency::Flags::NONE},
		_srcSubpass{&RenderPass::Subpass::External},
		_dstSubpass{&RenderPass::Subpass::External},
		_srcStage{Pipeline::StageBits::NONE},
		_dstStage{Pipeline::StageBits::NONE},
		_srcAccess{RenderPass::AccessBits::NONE},
		_dstAccess{RenderPass::AccessBits::NONE}
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

	RenderPass RenderPass::Create(Context& context){
		return RenderPass(context);
	}

	RenderPass::RenderPass(Context& context){
		_impl = std::make_unique<Impl>();
		_impl->context = context.getInternalContext();
	}

	RenderPass::~RenderPass(){
		_impl.reset();
	}

	struct NativeTranslation_{
		Internal::Graphics::RenderPassConfigInfo info;

		std::vector<VkAttachmentDescription> attachments;
		std::vector<VkSubpassDescription> subpasses;
		std::vector<VkSubpassDependency> dependencies;
		
		std::vector<uint32_t> preserveAttachmentBuffer;
		std::vector<VkAttachmentReference> referenceBuffer;
		std::list<VkAttachmentReference> depthStencilBuffer;

		NativeTranslation_() : 
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
			dependencies{},
			preserveAttachmentBuffer{},
			referenceBuffer{},
			depthStencilBuffer{}
		{}

		void translateAttachments(const std::vector<RenderPass::AttachmentDescription>& in){
			attachments.resize(in.size());

			for (std::size_t i=0; i<attachments.size(); i++){
				const auto& src = in[i];

				attachments[i] = VkAttachmentDescription{
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
		}

		void translateSubpasses(const std::vector<RenderPass::Subpass>& in, const std::vector<RenderPass::AttachmentDescription>& inAttachments){
			subpasses.resize(in.size());

			for (std::size_t i=0; i<subpasses.size(); i++){
				const auto& src = in[i];

				uint32_t inputAttachmentCount = 0;
				{
					const auto& input = src.getInputAttachments();
					inputAttachmentCount = static_cast<uint32_t>(input.size());

					for (const auto& attachment : input){
						VkImageLayout layout = TextureLayoutToVulkan(attachment.layout);
						uint32_t index = &attachment.attachment - inAttachments.data();

						referenceBuffer.emplace_back(
							VkAttachmentReference{
								.attachment = index,
								.layout = layout
							}
						);
					}
				}
				VkAttachmentReference* inputAttachments = referenceBuffer.data() + (referenceBuffer.size() - inputAttachmentCount);

				uint32_t colorAttachmentCount = 0;
				{
					const auto& input = src.getColorAttachments();
					colorAttachmentCount = static_cast<uint32_t>(input.size());

					for (const auto& attachment : input){
						VkImageLayout layout = TextureLayoutToVulkan(attachment.layout);
						uint32_t index = &attachment.attachment - inAttachments.data();

						referenceBuffer.emplace_back(
							VkAttachmentReference{
								.attachment = index,
								.layout = layout
							}
						);
					}
				}
				VkAttachmentReference* colorAttachments = referenceBuffer.data() + (referenceBuffer.size() - colorAttachmentCount);

				uint32_t preserveAttachmentCount = 0;
				{
					const auto& input = src.getPreserveAttachments();
					preserveAttachmentCount = static_cast<uint32_t>(input.size());

					for (const auto& attachment : input){
						uint32_t index = &attachment.get() - inAttachments.data();
						preserveAttachmentBuffer.push_back(index);
					}
				}
				uint32_t* preserveAttachments = preserveAttachmentBuffer.data() + (preserveAttachmentBuffer.size() - preserveAttachmentCount);

				VkAttachmentReference* depthStencil = nullptr;

				if (src.getDepthAttachment().has_value()){
					auto& attachment = src.getDepthAttachment();

					depthStencilBuffer.emplace_back(
						VkAttachmentReference{
							.attachment = uint32_t(&attachment->attachment - inAttachments.data()),
							.layout = TextureLayoutToVulkan(attachment->layout)
						}
					);

					depthStencil = &depthStencilBuffer.back();
				}

				subpasses[i] = VkSubpassDescription{
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
		}

		void translateDependencies(const std::vector<RenderPass::Dependency>& in, const std::vector<RenderPass::Subpass>& inSubpasses){
			dependencies.resize(in.size());

			for (std::size_t i=0; i<dependencies.size(); i++){
				const auto& src = in[i];

				uint32_t srcSubpass = &src.getSrcSubpass() == &RenderPass::Subpass::External ? VK_SUBPASS_EXTERNAL : static_cast<uint32_t>(&src.getSrcSubpass() - inSubpasses.data());
				uint32_t dstSubpass = &src.getDstSubpass() == &RenderPass::Subpass::External ? VK_SUBPASS_EXTERNAL : static_cast<uint32_t>(&src.getDstSubpass() - inSubpasses.data());

				dependencies[i] = VkSubpassDependency{
					.srcSubpass = srcSubpass,
					.dstSubpass = dstSubpass,
					.srcStageMask = pipelineStageFlagsToVulkan(src.getSrcStage()),
					.dstStageMask = pipelineStageFlagsToVulkan(src.getDstStage()),
					.srcAccessMask = AccessFlagsToVulkan(src.getSrcAccess()),
					.dstAccessMask = AccessFlagsToVulkan(src.getDstAccess()),
					.dependencyFlags = SubpassDependencyFlagsToVulkan(src.getFlags())
				};
			}
		}

		void updateNativeInfo(){
			info.attachmentCount = static_cast<uint32_t>(attachments.size());
			info.pAttachments = attachments.data();

			info.subpassCount = static_cast<uint32_t>(subpasses.size());
			info.pSubpasses = subpasses.data();

			info.dependencyCount = static_cast<uint32_t>(dependencies.size());
			info.pDependencies = dependencies.data();
		}

		void clear(){
			referenceBuffer.clear();
			preserveAttachmentBuffer.clear();
			depthStencilBuffer.clear();
		}
	};

	void RenderPass::initialize(){
		LOGGER->info("Initializing render pass ...");

		NativeTranslation_ translation;

		translation.translateAttachments(_impl->attachmentsDescriptions);
		translation.translateSubpasses(_impl->subpasses, _impl->attachmentsDescriptions);
		translation.translateDependencies(_impl->dependencies, _impl->subpasses);
		translation.updateNativeInfo();

		_impl->internal = std::make_shared<Internal::Graphics::RenderPass>(GRAPHICS_CONTEXT, translation.info);

		LOGGER->info("Render pass initialized with success !");
	}

	void RenderPass::release(){
		auto context = _impl->context;

		_impl = std::make_unique<Impl>();
		_impl->context = context;
	}

	bool RenderPass::isInitialized() const noexcept{
		return _impl->internal != nullptr;
	}

	void* RenderPass::getNativeHandle() const noexcept{
		return static_cast<void*>(_impl->internal->get());
	}

	RenderPass::Impl* RenderPass::getImpl() const noexcept{
		return _impl.get();
	}

	GUID RenderPass::getGUID() const noexcept{

	}

	RenderPass::AttachmentDescription& RenderPass::addAttachment(){
		return _impl->attachmentsDescriptions.emplace_back();
	}

	RenderPass::Subpass& RenderPass::addSubpass(){
		return _impl->subpasses.emplace_back();
	}

	RenderPass::Dependency& RenderPass::addDependency(){
		return _impl->dependencies.emplace_back();
	}

	void RenderPass::begin(CommandBuffer& commandBuffer){
		// VkRenderPassBeginInfo info{};
		// info.pClearValues
		
		// vkCmdBeginRenderPass()
	}
}