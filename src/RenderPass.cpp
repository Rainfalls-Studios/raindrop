#include <Raindrop/RenderPass.hpp>
#include <Raindrop/GUID.hpp>

namespace Raindrop{
	RenderPass RenderPass::Create(Context& context){
		
	}

	RenderPass::RenderPass(const RenderPass& other){

	}

	RenderPass& RenderPass::operator=(const RenderPass& other){

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