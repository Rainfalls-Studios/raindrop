#ifndef __RAINDROP_GARPHICS_BUILDER_FRAMEBUFFER_BUILDER_HPP__
#define __RAINDROP_GARPHICS_BUILDER_FRAMEBUFFER_BUILDER_HPP__

#include <Raindrop/Graphics/builders/common.hpp>

namespace Raindrop::Graphics::Builders{
	class FramebufferBuilder{
		public:
			FramebufferBuilder();
			~FramebufferBuilder();

			void setRenderPass(const std::shared_ptr<RenderPass>& renderPass);
			std::shared_ptr<RenderPass> renderPass() const;

			uint32_t attachmentCount() const;
			void setAttachment(uint32_t attachmentIndex, const std::shared_ptr<ImageView>& imageView);
			std::shared_ptr<ImageView> attachment(uint32_t attachmentIndex) const;

			void setSize(uint32_t width, uint32_t height = 1, uint32_t layers = 1);
			void setWidth(uint32_t width);
			void setHeight(uint32_t height);
			void setLayers(uint32_t layers);
			uint32_t width() const;
			uint32_t height() const;
			uint32_t layers() const;

		private:
			std::shared_ptr<RenderPass> _renderPass;
			std::vector<std::shared_ptr<ImageView>> _imageViews;

			uint32_t _width;
			uint32_t _height;
			uint32_t _layers;

	};
}

#endif