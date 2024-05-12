#include <Raindrop/RenderPass.hpp>
#include <Raindrop/GUID.hpp>

namespace Raindrop{
	//--------------------------------------------------------------------
	//-----------------           Attachment             -----------------
	//--------------------------------------------------------------------

	RenderPass::Subpass::AttachmentsInfo::AttachmentsInfo(){}

	RenderPass::Subpass::AttachmentsInfo::AttachmentsInfo(const std::size_t& index){
		_indices.push_back(index);
	}

	RenderPass::Subpass::AttachmentsInfo::AttachmentsInfo(const std::initializer_list<std::size_t>& indices){
		for (const auto& index : indices){
			_indices.push_back(index);
		}
	}

	const std::list<std::size_t>& RenderPass::Subpass::AttachmentsInfo::getIndices() const noexcept{
		return _indices;
	}

	//--------------------------------------------------------------------
	//-----------------          Render pass             -----------------
	//--------------------------------------------------------------------

	RenderPass RenderPass::Create(Context& context){
		
	}

	RenderPass::RenderPass(const RenderPass& other){

	}

	RenderPass& RenderPass::operator=(const RenderPass& other){

		return *this;
	}

	void RenderPass::initialize(){

	}

	void RenderPass::release(){

	}


	bool RenderPass::isInitialized() const noexcept{

	}

	void* RenderPass::getNativeHandle() const noexcept{

	}

	RenderPass::Handle RenderPass::getHandle() const noexcept{
		return _handle;
	}

	GUID RenderPass::getGUID() const noexcept{

	}

	// InstanceID getInstanceID() const noexcept; TODO: unique runtime engine scope id

	void RenderPass::addAttachment(const std::size_t& index, const Format& format, const Texture::Operation& operation, const Texture::Layout& initLayout, const Texture::Layout& finalLayout){

	}

	RenderPass::Subpass RenderPass::addSubpass(){

	}
}