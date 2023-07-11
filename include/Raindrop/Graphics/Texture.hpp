#ifndef __RAINDROP_GRAPHICS_TEXTURE_HPP__
#define __RAINDROP_GRAPHICS_TEXTURE_HPP__

#include <Raindrop/Graphics/common.hpp>

namespace Raindrop::Graphics{
	class Texture{
		public:
			Texture(GraphicsContext& context, const std::filesystem::path &path);
			~Texture();

		private:
			void createImage(const std::filesystem::path &filepath);
			void createImageView();
			void createSampler();

			GraphicsContext& _context;

			VkImage _image = VK_NULL_HANDLE;
			VkDeviceMemory _memory = VK_NULL_HANDLE;
			VkImageView _imageView = VK_NULL_HANDLE;
			VkSampler _sampler = VK_NULL_HANDLE;

			uint32_t _width;
			uint32_t _height;

			void transitionToTransfert(VkCommandBuffer commandBuffer);
			void transitionToShaderRead(VkCommandBuffer commandBuffer);
			void copyBuffer(VkCommandBuffer commandBuffer, Buffer& buffer);

	};
}

#endif