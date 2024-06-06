#ifndef __RAINDROP_INTERNAL_GRAPHICS_IMAGE_HPP__
#define __RAINDROP_INTERNAL_GRAPHICS_IMAGE_HPP__

#include <Raindrop_internal/Graphics/Image.hpp>
#include <Raindrop_internal/Graphics/Context.hpp>

namespace Raindrop::Internal::Graphics{
	class Image{
		public:
			Image(Context& context, const VkImageCreateInfo& info);
			~Image();

			VkImage get() const;
			VkDeviceMemory memory() const;

			const VkFormat& getFormat() const noexcept;
			const std::uint32_t& getWidth() const noexcept;
			const std::uint32_t& getHeight() const noexcept;
			const std::uint32_t& getDepth() const noexcept;
			const VkImageUsageFlags& getUsage() const noexcept;
			const VkImageLayout& getLayout() const noexcept;
			const VkImageTiling& getTiling() const noexcept;
			const VkImageType& getType() const noexcept;
			const std::uint32_t& getMipCount() const noexcept;
			const std::uint32_t& getArrLayers() const noexcept;
			const VkImageCreateFlags& getFlags() const noexcept;

		private:
			Context& _context;

			VkImage _image;
			VkDeviceMemory _memory;

			VkFormat _format;
			std::uint32_t _width;
			std::uint32_t _height;
			std::uint32_t _depth;
			VkImageUsageFlags _usage;
			VkImageLayout _layout;
			VkImageTiling _tiling;
			VkImageType _type;
			std::uint32_t _mipCount;
			std::uint32_t _arrLayers;
			VkImageCreateFlags _flags;
	};
}

#endif