#ifndef __RAINDROP_RENDERER_TEXTURE_TEXTURE_HPP__
#define __RAINDROP_RENDERER_TEXTURE_TEXTURE_HPP__

#include "common.hpp"
#include <Raindrop/Core/Asset/Asset.hpp>

namespace Raindrop::Renderer::Texture{
	class Texture : public ::Raindrop::Core::Asset::Asset{
		public:
			Texture(Context& context, const std::filesystem::path& path);
			virtual ~Texture();

			VkImage image();
			VkImageView imageView();
			VkDeviceMemory memory();

		private:
			Context& _context;

			VkImage _image;
			VkImageView _view;
			VkDeviceMemory _memory;
	};
}

#endif