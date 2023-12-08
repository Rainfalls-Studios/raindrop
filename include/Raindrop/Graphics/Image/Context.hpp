#ifndef __RAINDROP_GRAPHICS_IMAGE_CONTEXT_HPP__
#define __RAINDROP_GRAPHICS_IMAGE_CONTEXT_HPP__

#include <spdlog/spdlog.h>

#include <Raindrop/Graphics/Image/common.hpp>
#include <Raindrop/Graphics/Context.hpp>
#include <Raindrop/Graphics/Image/ImageManager.hpp>
#include <Raindrop/Graphics/Image/ImageViewManager.hpp>
#include <Raindrop/Graphics/Image/Loader.hpp>

namespace Raindrop::Graphics::Image{
	class Context{
		public:
			Context(Graphics::Context& graphics);
			~Context();

			Graphics::Context& graphics();
			spdlog::logger& logger();
			ImageManager& imageManager();
			ImageViewManager& imageViewManager();
			Loader& loader();

		private:
			Graphics::Context& _graphics;
			std::shared_ptr<spdlog::logger> _logger;

			std::unique_ptr<ImageManager> _imageManager;
			std::unique_ptr<ImageViewManager> _imageViewManager;
			std::unique_ptr<Loader> _loader;
	};
}

#endif