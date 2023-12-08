#include <Raindrop/Graphics/Image/ImageViewManager.hpp>
#include <Raindrop/Graphics/Image/ImageView.hpp>
#include <Raindrop/Graphics/Image/ImageViewBuilder.hpp>

namespace Raindrop::Graphics::Image{
	ImageViewManager::ImageViewManager(Context& context) : _context{context}{}
	ImageViewManager::~ImageViewManager(){}

	std::shared_ptr<ImageView> ImageViewManager::create(ImageViewBuilder& builder){
		std::shared_ptr<ImageView> imageView = std::make_shared<ImageView>(_context, builder);
		_imageViews[builder.name()] = imageView;
		return imageView;
	}

	std::shared_ptr<ImageView> ImageViewManager::get(std::string name){
		return _imageViews[name];
	}

	void ImageViewManager::clean(){
		auto it = _imageViews.begin();

		while (it != _imageViews.end()){
			if (it->second.unique()){
				_imageViews.erase(it);
				continue;
			}
			it++;
		}
	}
}