#ifndef __RAINDROP_GRAPHICS_FRAMEBUFFER_FRAMEBUFER_BUILDER_HPP__
#define __RAINDROP_GRAPHICS_FRAMEBUFFER_FRAMEBUFER_BUILDER_HPP__

#include <Raindrop/Graphics/Framebuffer/common.hpp>
#include <Raindrop/Graphics/RenderPass/common.hpp>
#include <Raindrop/Graphics/Image/common.hpp>

namespace Raindrop::Graphics::Framebuffer{
	class FramebufferBuilder{
		public:
			FramebufferBuilder(Context& context);
			~FramebufferBuilder();

			void loadFromNode(const YAML::Node& node);

			const std::string& name() const;
			VkFramebufferCreateFlags flags() const;
			const std::shared_ptr<RenderPass::RenderPass>& renderPass() const;
			const std::vector<std::shared_ptr<Image::ImageView>>& attachments() const;

			uint32_t width() const;
			uint32_t height() const;
			uint32_t layers() const;


		private:
			Context& _context;

			std::string _name;
			VkFramebufferCreateFlags _flags;
			std::shared_ptr<RenderPass::RenderPass> _renderPass;
			std::vector<std::shared_ptr<Image::ImageView>> _attachments;

			uint32_t _width;
			uint32_t _height;
			uint32_t _layers;

			void loadAttachment(const YAML::Node& node);
	};
}

#endif