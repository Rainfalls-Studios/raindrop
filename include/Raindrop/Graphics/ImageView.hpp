#ifndef __RAINDROP_GRAPHICS_IMAGE_VIEW_HPP__
#define __RAINDROP_GRAPHICS_IMAGE_VIEW_HPP__

#include "types.hpp"
#include "pch.pch"
#include <Raindrop/Object.hpp>

namespace Raindrop::Graphics{
	class ImageView : public Object{
		public:
			ImageView() noexcept;
			~ImageView();

			ImageView(const ImageView&) = delete;
			ImageView& operator=(const ImageView&) = delete;

			ImageView(ImageView&& other);
			ImageView& operator=(ImageView&& other);

			friend void swap(ImageView& A, ImageView& B);

			void prepare(Context& context);
			void initialize();
			void release();

			ImageView& setSource(const Image& image);
			ImageView& setSource(const VkImage& image);
			ImageView& setFlags(const VkImageViewCreateFlags& flags);
			ImageView& setType(const VkImageViewType& type);
			ImageView& setFormat(const VkFormat& format);
			ImageView& setComponentMapping(const VkComponentMapping& mapping);
			ImageView& setAspectMask(const VkImageAspectFlags& mask);
			ImageView& setBaseMipmapLevel(const std::uint32_t& level);
			ImageView& setMipmapLevelCount(const std::uint32_t& count);
			ImageView& setBaseArrayLayer(const std::uint32_t& layer);
			ImageView& setLayerCount(const std::uint32_t& count);

			const VkImageView& get() const noexcept;

		private:
			struct BuildInfo{
				VkImageViewCreateInfo info;

				BuildInfo();
			};

			Context* _context;
			VkImageView _imageView;

			std::unique_ptr<BuildInfo> _info;
			BuildInfo& getInfo();
	};
}

#endif