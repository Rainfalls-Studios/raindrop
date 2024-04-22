#ifndef __RAINDROP_GRAPHICS_TEXTURE_TEXTURE_HPP__
#define __RAINDROP_GRAPHICS_TEXTURE_TEXTURE_HPP__

#include "common.hpp"
#include <Raindrop/Core/Assets/Asset.hpp>

namespace Raindrop::Graphics::Textures{
	class Texture : public ::Raindrop::Core::Assets::Asset{
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

			const std::shared_ptr<Image>& image() const;
			const std::shared_ptr<ImageView>& imageView() const;
			const std::shared_ptr<Sampler>& sampler() const;

			VkDescriptorImageInfo info() const;

		private:
			Context& _context;

			Maths::uvec2 _size;
			std::shared_ptr<Image> _image;
			std::shared_ptr<ImageView> _imageView;
			std::shared_ptr<Sampler> _sampler;

			void createImage();
			void createSampler();
			void createImageView();

			void loadFromConstructData(const ConstructData& data);
	};
}

#endif