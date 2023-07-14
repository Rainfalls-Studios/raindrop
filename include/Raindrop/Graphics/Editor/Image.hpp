#ifndef __RAINDROP_GRAPHICS_EDITOR_IMAGE_HPP__
#define __RAINDROP_GRAPHICS_EDITOR_IMAGE_HPP__

#include <Raindrop/Graphics/Editor/common.hpp>
#include <Raindrop/Graphics/Texture.hpp>

namespace Raindrop::Graphics::Editor{
	class Image{
		public:
			Image(EditorContext& context);
			virtual ~Image();

			ImTextureID texture() const;
			virtual uint32_t width() const = 0;
			virtual uint32_t height() const = 0;

		protected:
			void setTexture(VkSampler sampler, VkImageView imageView, VkImageLayout layout);
			void removeTexture();

			EditorContext& _context;
			VkDescriptorSet _descriptorSet = VK_NULL_HANDLE;
	};
}

#endif