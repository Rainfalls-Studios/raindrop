// #include <Raindrop/Graphics/Builders/FramebufferBuilder.hpp>
// #include <Raindrop/Graphics/RenderPass/RenderPass.hpp>

// namespace Raindrop::Graphics::Builders{
// 	FramebufferBuilder::FramebufferBuilder(){
// 		_renderPass = nullptr;
// 		_width = 1;
// 		_height = 1;
// 		_layers = 1;
// 	}

// 	FramebufferBuilder::~FramebufferBuilder(){}

// 	void FramebufferBuilder::setRenderPass(const std::shared_ptr<RenderPass>& renderPass){
// 		_renderPass = renderPass;

// 		_imageViews.resize(_renderPass ? _renderPass->attachmentCounts() : 0);
// 		std::fill(_imageViews.begin(), _imageViews.end(), nullptr);
// 	}

// 	std::shared_ptr<RenderPass> FramebufferBuilder::renderPass() const{
// 		return _renderPass;
// 	}

// 	uint32_t FramebufferBuilder::attachmentCount() const{
// 		return _imageViews.size();
// 	}

// 	void FramebufferBuilder::setAttachment(uint32_t attachmentIndex, const std::shared_ptr<ImageView>& imageView){
// 		_imageViews[attachmentIndex] = imageView;
// 	}

// 	std::shared_ptr<ImageView> FramebufferBuilder::attachment(uint32_t attachmentIndex) const{
// 		auto& imageView = _imageViews[attachmentIndex];
// 		if (imageView){
// 			return imageView;
// 		} else {
// 			return _renderPass->createDefaultAttachmentImage(attachmentIndex);
// 		}
// 	}

// 	void FramebufferBuilder::setSize(uint32_t width, uint32_t height, uint32_t layers){
// 		_width = width;
// 		_height = height;
// 		_layers = layers;
// 	}

// 	void FramebufferBuilder::setWidth(uint32_t width){
// 		_width = width;
// 	}

// 	void FramebufferBuilder::setHeight(uint32_t height){
// 		_height = height;
// 	}

// 	void FramebufferBuilder::setLayers(uint32_t layers){
// 		_layers = layers;
// 	}

// 	uint32_t FramebufferBuilder::width() const{
// 		return _width;
// 	}

// 	uint32_t FramebufferBuilder::height() const{
// 		return _height;
// 	}

// 	uint32_t FramebufferBuilder::layers() const{
// 		return _layers;
// 	}
// }