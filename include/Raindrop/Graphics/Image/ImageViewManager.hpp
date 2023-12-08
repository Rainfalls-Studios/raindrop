#ifndef __RAINDROP_GRAPHICS_IMAGE_IMAGE_VIEW_MANAGER_HPP__
#define __RAINDROP_GRAPHICS_IMAGE_IMAGE_VIEW_MANAGER_HPP__

#include <Raindrop/Graphics/Image/common.hpp>

namespace Raindrop::Graphics::Image{
	class ImageViewManager{
		public:
			ImageViewManager(Context& context);
			~ImageViewManager();

			std::shared_ptr<ImageView> create(ImageViewBuilder& builder);
			std::shared_ptr<ImageView> get(std::string id);
			void clean();
		
		private:
			Context& _context;
			std::unordered_map<std::string, std::shared_ptr<ImageView>> _imageViews;

	};
}

#endif