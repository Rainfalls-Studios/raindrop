#ifndef __RAINDROP_GRAPHICS_IMAGE_IMAGE_MANAGER_HPP__
#define __RAINDROP_GRAPHICS_IMAGE_IMAGE_MANAGER_HPP__

#include <Raindrop/Graphics/Image/common.hpp>

namespace Raindrop::Graphics::Image{
	class ImageManager{
		public:
			ImageManager(Context& context);
			~ImageManager();

			std::shared_ptr<Image> create(ImageBuilder& builder);
			std::shared_ptr<Image> get(std::string name);
			void clean();

		private:
			Context& _context;
			std::unordered_map<std::string, std::shared_ptr<Image>> _images;

	};
}

#endif