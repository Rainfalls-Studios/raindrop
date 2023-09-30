#ifndef __RAINDROP_GRAPHICS_RENDER_PASS_HPP__
#define __RAINDROP_GRAPHICS_RENDER_PASS_HPP__

#include <Raindrop/Graphics/common.hpp>
#include <Raindrop/Graphics/builders/common.hpp>

namespace Raindrop::Graphics{
	class RenderPass{
		public:
			virtual ~RenderPass() = default;

			virtual VkRenderPass get() const = 0;
			virtual uint32_t attachmentCounts() const = 0;

			virtual bool begin(VkCommandBuffer commandBuffer) = 0;
			virtual void end(VkCommandBuffer commandBuffer) = 0;

			virtual std::shared_ptr<ImageView> createDefaultAttachmentImage(uint32_t attachment) const = 0;
	};
}

#endif