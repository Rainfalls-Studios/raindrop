#ifndef __RAINDROP_GRAPHICS_IMAGE_IMAGE_VIEW_BUILDER_HPP__
#define __RAINDROP_GRAPHICS_IMAGE_IMAGE_VIEW_BUILDER_HPP__

#include <Raindrop/Graphics/Image/common.hpp>
#include <Raindrop/Core/Registry/common.hpp>

namespace Raindrop::Graphics::Image{
	class ImageViewBuilder{
		public:
			ImageViewBuilder(Context& context);
			~ImageViewBuilder();

			ImageViewBuilder& operator=(const ImageViewBuilder& other);

			void loadFromNode(const YAML::Node& node);

			void setImage(const std::shared_ptr<Image>& image);
			std::shared_ptr<Image> image() const;

			void setViewType(VkImageViewType type);
			VkImageViewType viewType() const;

			void setFormat(VkFormat format);
			VkFormat format() const;

			void setComponentMapping(VkComponentMapping mapping);
			VkComponentMapping componentMapping() const;

			void setSubResourceRange(VkImageSubresourceRange subResource);
			VkImageSubresourceRange subResourceRange() const;

			std::string& name();
			const std::string& name() const;
			void setName(const std::string& name);

		private:
			Context& _context;
			
			std::shared_ptr<Image> _image;
			VkImageViewType _viewType;
			VkFormat _format;
			VkComponentMapping _componentMapping;
			VkImageSubresourceRange _subResource;
			std::string _name;
	};
}

#endif