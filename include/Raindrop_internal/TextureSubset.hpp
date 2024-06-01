#ifndef __RAINDROP_INTERNAL_TEXTURE_SUBSET_HPP__
#define __RAINDROP_INTERNAL_TEXTURE_SUBSET_HPP__

#include <Raindrop/TextureSubset.hpp>
#include <Raindrop_internal/Graphics/ImageView.hpp>

namespace Raindrop{
	struct TextureSubset::Impl{
		std::shared_ptr<Internal::Graphics::ImageView> imageView;
		Internal::Graphics::ImageViewConfigInfo info;
		Context* context;

		Impl(Context& context) : context{&context}{}
	};

	VkImageViewType toVulkan(const TextureSubset::Type& type);
	VkImageViewCreateFlags toVulkan(const TextureSubset::Flags& flags);

}

#endif