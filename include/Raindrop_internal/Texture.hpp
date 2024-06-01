#ifndef __RAINDROP_INTERNAL_TEXTURE_HPP__
#define __RAINDROP_INTERNAL_TEXTURE_HPP__

#include <Raindrop/Texture.hpp>
#include <Raindrop_internal/Graphics/Image.hpp>

namespace Raindrop{
	struct Texture::Impl{
		std::shared_ptr<Internal::Graphics::Image> image;
		Internal::Graphics::ImageConfigInfo info;
		Context* context;

		Impl(Context& context) : context{&context}{}
	};

	VkImageAspectFlags toVulkan(const Texture::Aspect& a);
	VkImageTiling toVulkan(const Texture::Tiling& t);
	VkImageLayout toVulkan(const Texture::Layout& layout);
	VkImageType toVulkan(const Texture::Type& t);
	VkImageUsageFlags toVulkan(const Texture::Usage& u);
	VkImageCreateFlags toVulkan(const Texture::Flags& flags);
}

#endif