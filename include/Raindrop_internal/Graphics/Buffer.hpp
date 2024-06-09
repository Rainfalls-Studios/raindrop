#ifndef __RAINDROP_INTERNAL_GRAPHICS_BUFFER_HPP__
#define __RAINDROP_INTERNAL_GRAPHICS_BUFFER_HPP__

#include "common.hpp"

namespace Raindrop::Internal::Graphics{
	class Buffer{
		public:
			Buffer(Context& context);
			~Buffer();

			Buffer(const Buffer&) = delete;
			Buffer& operator=(const Buffer&) = delete;

			void allocate(VkDeviceSize size, VkBufferUsageFlags usageFlags, VkBufferCreateFlags createFlags, VkMemoryPropertyFlags memoryPropertyFlags, VkDeviceSize minOffsetAlignment = 1);

			void free();

			VkResult map(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);
  			void unmap();

			void write(void* data, VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);
			VkResult flush(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);
			VkDescriptorBufferInfo descriptorInfo(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);
			VkResult invalidate(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);

			VkBuffer get();
			VkDeviceMemory memory();
			void* mapped();

			const std::size_t& getSize() const noexcept;
			const std::size_t& getMinAllignement() const noexcept;
			const VkBufferUsageFlags& getUsageFlags() const noexcept;
			const VkBufferCreateFlags& getCreateFlags() const noexcept;
			const VkMemoryPropertyFlags& getMemoryPropertyFlags() const noexcept;

		private:
			Context& _context;

			VkDeviceMemory _memory;
			VkBuffer _buffer;
			void* _mapped;

			std::size_t _size;
			std::size_t _minAllignement;
			VkBufferUsageFlags _usageFlags;
			VkBufferCreateFlags _createFlags;
			VkMemoryPropertyFlags _memoryPropertyFlags;

	};
}

#endif