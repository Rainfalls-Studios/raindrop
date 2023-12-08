#include <Raindrop/Graphics/Image/ImageManager.hpp>
#include <Raindrop/Graphics/Image/Image.hpp>
#include <Raindrop/Graphics/Image/ImageBuilder.hpp>

namespace Raindrop::Graphics::Image{
	ImageManager::ImageManager(Context& context) : _context{context}{}
	ImageManager::~ImageManager(){}

	std::shared_ptr<Image> ImageManager::create(ImageBuilder& builder){
		std::shared_ptr<Image> image = std::make_shared<Image>(_context, builder);
		_images[builder.name()] = image;
		return image;
	}

	std::shared_ptr<Image> ImageManager::get(std::string name){
		return _images[name];
	}

	void ImageManager::clean(){
		auto it = _images.begin();

		while (it != _images.end()){
			if (it->second.unique()){
				_images.erase(it);
				continue;
			}
			it++;
		}
	}

}