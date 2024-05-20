#include <Raindrop/RenderPass.hpp>
#include <Raindrop/GUID.hpp>
#include <Raindrop_internal/RenderPass.hpp>
#include <Raindrop_internal/Graphics/Context.hpp>
#include <Raindrop_internal/Context.hpp>
#include <Raindrop_internal/Graphics/Renderer.hpp>


#define LOGGER _impl->context->getInternalContext()->getLogger()
#define INFO _impl->info
#define GRAPHICS_CONTEXT _impl->context->getInternalContext()->getRenderer().getContext()
#define ATTACHMENT_DATA reinterpret_cast<VkAttachmentDescription*>(_data)
#define SUBPASS_DATA reinterpret_cast<Raindrop::Internal::Graphics::RenderPassConfigInfo::SubpassInfo*>(_data)
#define DEPENDENCY_DATA reinterpret_cast<VkSubpassDependency*>(_data)

namespace Raindrop{
	RenderPass::Attachment::Attachment(RenderPass& owner, void* data) : _owner{owner}, _data{data}{
		ATTACHMENT_DATA->samples = VK_SAMPLE_COUNT_1_BIT;
	}

	RenderPass::Attachment& RenderPass::Attachment::setFlags(const Flags& flags){
		ATTACHMENT_DATA->flags = static_cast<VkAttachmentDescriptionFlags>(flags.get());
		return *this;
	}

	RenderPass::Attachment& RenderPass::Attachment::setFormat(const Format& format){
		ATTACHMENT_DATA->format = static_cast<VkFormat>(format.get());
		return *this;
	}

	RenderPass::Attachment& RenderPass::Attachment::setLoadOperation(const Operation::Load& operation){
		ATTACHMENT_DATA->loadOp = static_cast<VkAttachmentLoadOp>(operation);
		return *this;
	}

	RenderPass::Attachment& RenderPass::Attachment::setStoreOperation(const Operation::Store& operation){
		ATTACHMENT_DATA->storeOp = static_cast<VkAttachmentStoreOp>(operation);
		return *this;
	}

	RenderPass::Attachment& RenderPass::Attachment::setStencilLoadOperation(const Operation::Load& operation){
		ATTACHMENT_DATA->stencilLoadOp = static_cast<VkAttachmentLoadOp>(operation);
		return *this;
	}

	RenderPass::Attachment& RenderPass::Attachment::setStencilStoreOperation(const Operation::Store& operation){
		ATTACHMENT_DATA->stencilStoreOp = static_cast<VkAttachmentStoreOp>(operation);
		return *this;
	}

	RenderPass::Attachment& RenderPass::Attachment::setInitialLayout(const Texture::Layout& layout){
		ATTACHMENT_DATA->initialLayout = static_cast<VkImageLayout>(layout);
		return *this;
	}

	RenderPass::Attachment& RenderPass::Attachment::setFinalLayout(const Texture::Layout& layout){
		ATTACHMENT_DATA->finalLayout = static_cast<VkImageLayout>(layout);
		return *this;
	}

	//--------------------------------------------------------------------
	//-----------------            Subpass               -----------------
	//--------------------------------------------------------------------

	const RenderPass::Subpass RenderPass::Subpass::External = RenderPass::Subpass(nullptr, nullptr);

	RenderPass::Subpass::Subpass(RenderPass* owner, void* data) : _owner{owner}, _data{data}{
		if (_data){
			SUBPASS_DATA->description.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		}
	}

	RenderPass::Subpass& RenderPass::Subpass::setFlags(const Flags& flags){
		SUBPASS_DATA->description.flags = static_cast<VkSubpassDescriptionFlags>(flags.get());
		return *this;
	}

	RenderPass::Subpass& RenderPass::Subpass::setDepthAttachment(const AttachmentReference& attachment){
		std::size_t index = static_cast<std::size_t>(reinterpret_cast<VkAttachmentDescription*>(attachment.attachment._data) - &_owner->_impl->info.attachments[0]);

		VkAttachmentReference ref{
			.attachment = static_cast<uint32_t>(index),
			.layout = static_cast<VkImageLayout>(attachment.layout)
		};

		SUBPASS_DATA->depth = ref;
		return *this;
	}

	RenderPass::Subpass& RenderPass::Subpass::addColorAttachment(const AttachmentReference& attachment){
		std::size_t index = static_cast<std::size_t>(reinterpret_cast<VkAttachmentDescription*>(attachment.attachment._data) - &_owner->_impl->info.attachments[0]);

		VkAttachmentReference ref{
			.attachment = static_cast<uint32_t>(index),
			.layout = static_cast<VkImageLayout>(attachment.layout)
		};

		SUBPASS_DATA->color.push_back(ref);
		return *this;
	}

	RenderPass::Subpass& RenderPass::Subpass::addInputAttachment(const AttachmentReference& attachment){
		std::size_t index = static_cast<std::size_t>(reinterpret_cast<VkAttachmentDescription*>(attachment.attachment._data) - &_owner->_impl->info.attachments[0]);

		VkAttachmentReference ref{
			.attachment = static_cast<uint32_t>(index),
			.layout = static_cast<VkImageLayout>(attachment.layout)
		};

		SUBPASS_DATA->input.push_back(ref);
		return *this;
	}

	RenderPass::Subpass& RenderPass::Subpass::addPreserveAttachment(const Attachment& attachment){
		std::size_t index = static_cast<std::size_t>(reinterpret_cast<VkAttachmentDescription*>(attachment._data) - &_owner->_impl->info.attachments[0]);
		SUBPASS_DATA->preserve.push_back(index);
		return *this;
	}


	//--------------------------------------------------------------------
	//-----------------           Dependency             -----------------
	//--------------------------------------------------------------------

	RenderPass::Dependency::Dependency(RenderPass& owner, void* data) : _owner{owner}, _data{data}{
	}


	RenderPass::Dependency& RenderPass::Dependency::setFlags(const Flags& flags){
		DEPENDENCY_DATA->dependencyFlags = static_cast<VkDependencyFlags>(flags.get());
		return *this;
	}

	RenderPass::Dependency& RenderPass::Dependency::setSrcSubpass(const Subpass& subpass){
		uint32_t index = 0;
		if (subpass._data == nullptr){
			index = VK_SUBPASS_EXTERNAL;
		} else {
			index = static_cast<uint32_t>(reinterpret_cast<Internal::Graphics::RenderPassConfigInfo::SubpassInfo*>(subpass._data) - &_owner._impl->info.subpasses[0]);
		}

		DEPENDENCY_DATA->srcSubpass = index;
		return *this;
	}

	RenderPass::Dependency& RenderPass::Dependency::setDstSubpass(const Subpass& subpass){
		uint32_t index = 0;
		if (subpass._data == nullptr){
			index = VK_SUBPASS_EXTERNAL;
		} else {
			index = static_cast<uint32_t>(reinterpret_cast<Internal::Graphics::RenderPassConfigInfo::SubpassInfo*>(subpass._data) - &_owner._impl->info.subpasses[0]);
		}

		DEPENDENCY_DATA->dstSubpass = index;
		return *this;
	}

	RenderPass::Dependency& RenderPass::Dependency::setSrcStage(const Pipeline::Stage& stage){
		DEPENDENCY_DATA->srcStageMask = static_cast<VkPipelineStageFlags>(stage.get());
		return *this;
	}

	RenderPass::Dependency& RenderPass::Dependency::setDstStage(const Pipeline::Stage& stage){
		DEPENDENCY_DATA->dstStageMask = static_cast<VkPipelineStageFlags>(stage.get());
		return *this;
	}

	RenderPass::Dependency& RenderPass::Dependency::setSrcAccess(const Access& access){
		DEPENDENCY_DATA->srcAccessMask = static_cast<VkAccessFlags>(access.get());
		return *this;
	}

	RenderPass::Dependency& RenderPass::Dependency::setDstAccess(const Access& access){
		DEPENDENCY_DATA->dstAccessMask = static_cast<VkAccessFlags>(access.get());
		return *this;
	}

	//--------------------------------------------------------------------
	//-----------------          Render pass             -----------------
	//--------------------------------------------------------------------

	RenderPass RenderPass::Create(Context& context){
		return RenderPass(context);
	}

	RenderPass::RenderPass(Context& context){
		_impl = new Impl(context);
	}

	RenderPass::RenderPass(const RenderPass& other){
		_impl = new Impl();
		*_impl = *other._impl;
	}

	RenderPass& RenderPass::operator=(const RenderPass& other){
		*_impl = *other._impl;
		return *this;
	}

	RenderPass::~RenderPass(){
		delete _impl;
	}

	void RenderPass::initialize(){
		_impl->renderPass = std::make_shared<Internal::Graphics::RenderPass>(GRAPHICS_CONTEXT, INFO);
	}

	void RenderPass::release(){
		_impl->renderPass.reset();
	}

	bool RenderPass::isInitialized() const noexcept{
		return _impl->renderPass != nullptr;
	}

	void* RenderPass::getNativeHandle() const noexcept{
		return static_cast<void*>(_impl->renderPass->get());
	}

	RenderPass::Impl* RenderPass::getImpl() const noexcept{
		return _impl;
	}

	GUID RenderPass::getGUID() const noexcept{

	}

	RenderPass::Attachment RenderPass::addAttachment(){
		INFO.attachments.push_back({});
		VkAttachmentDescription& description = INFO.attachments[INFO.attachments.size() - 1];
		return Attachment(*this, &description);
	}

	RenderPass::Subpass RenderPass::addSubpass(){
		INFO.subpasses.push_back({});
		Internal::Graphics::RenderPassConfigInfo::SubpassInfo& info = INFO.subpasses[INFO.subpasses.size() - 1];
		return Subpass(this, &info);
	}

	RenderPass::Dependency RenderPass::addDependency(){
		INFO.dependencies.push_back({});
		VkSubpassDependency& description = INFO.dependencies[INFO.dependencies.size() - 1];
		return Dependency(*this, &description);
	}
}