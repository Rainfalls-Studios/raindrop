#ifndef __RAINDROP_GRAPHICS_TARGET_HPP__
#define __RAINDROP_GRAPHICS_TARGET_HPP__

#include <Raindrop/Graphics/common.hpp>

namespace Raindrop::Graphics{
	class Target{
		public:
			Target() = default;
			virtual ~Target() = default;

			virtual VkRenderPass renderPass() const = 0;
			virtual uint32_t attachmentCount() const = 0;

			virtual VkDescriptorImageInfo getAttachmentInfo(uint32_t attachment) const = 0;
			virtual VkDescriptorSet descriptorSet() const = 0;

			virtual float aspectRatio() const = 0;
			virtual uint32_t width() const = 0;
			virtual uint32_t height() const = 0;

			virtual bool beginRenderPass(VkCommandBuffer commandBuffer) = 0;
			virtual void endRenderPass(VkCommandBuffer commandBuffer) = 0;
	};
}

#endif