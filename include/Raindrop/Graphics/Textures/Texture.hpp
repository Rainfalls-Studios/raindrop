#ifndef __RAINDROP_GRAPHICS_TEXTURE_TEXTURE_HPP__
#define __RAINDROP_GRAPHICS_TEXTURE_TEXTURE_HPP__

#include "common.hpp"
#include <Raindrop/Core/Asset/Asset.hpp>

namespace Raindrop::Graphics::Textures{
	class Texture : public ::Raindrop::Core::Asset::Asset{
		public:
			struct ConstructData{
				std::size_t width;
				std::size_t height;
				std::size_t channels;
				const void* data;
			};

			Texture(Context& context, const std::filesystem::path& path);
			Texture(Context& context, const ConstructData& data);

			virtual ~Texture();

			VkImage image() const;
			VkImageView imageView() const;
			VkDeviceMemory memory() const;
			VkSampler sampler() const;

			VkDescriptorImageInfo info() const;

		private:
			Context& _context;

			VkImage _image;
			VkImageView _view;
			VkSampler _sampler;
			VkDeviceMemory _memory;

			void createSampler();
			void createImageView();

			void loadFromConstructData(const ConstructData& data);
	};
}

#endif