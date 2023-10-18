#ifndef __RAINDROP_GRAPHICS_BUFFERS_BUFFER_HPP__
#define __RAINDROP_GRAPHICS_BUFFERS_BUFFER_HPP__

#include <Raindrop/Graphics/Buffers/common.hpp>

namespace Raindrop::Graphics::Buffers{
	class Buffer{
		public:
			Buffer(Context& context);
			~Buffer();

			Buffer(const Buffer&) = delete;
			Buffer& operator=(const Buffer&) = delete;

			void allocateInstances(VkDeviceSize instanceSize, uint32_t instanceCount, VkBufferUsageFlags usageFlags, VkMemoryPropertyFlags memoryPropertyFlags, VkDeviceSize minOffsetAlignment = 1);
			void allocate(VkDeviceSize size, VkBufferUsageFlags usageFlags, VkMemoryPropertyFlags memoryPropertyFlags, VkDeviceSize minOffsetAlignment = 1);

			void free();

			VkResult map(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);
  			void unmap();

			void write(void* data, VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);
			VkResult flush(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);
			VkDescriptorBufferInfo descriptorInfo(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);
			VkResult invalidate(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);

			void writeToIndex(void* data, int index);
			VkResult flushIndex(int index);
			VkDescriptorBufferInfo descriptorInfoForIndex(int index);
			VkResult invalidateIndex(int index);

			VkBuffer get() const;
			VkDeviceMemory memory() const;
			void* mapped();

			void update(VkCommandBuffer commandBuffer, void* data, VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);
			void copy(VkCommandBuffer commandBuffer, VkBuffer destination, VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize sourceOffset = 0, VkDeviceSize destinationOffset = 0);
			void copyToImage();

			uint32_t instanceCount() const;
			uint32_t instanceSize() const;

			operator VkBuffer() const;

		private:
			Context& _context;
			VkDeviceMemory _memory = VK_NULL_HANDLE;
			VkBuffer _buffer = VK_NULL_HANDLE;
			void* _mapped = nullptr;

			uint32_t _instanceCount;
			VkDeviceSize _instanceSize;
			VkDeviceSize _alignmentSize;
			VkBufferUsageFlags _usageFlags;
			VkMemoryPropertyFlags _memoryPropertyFlags;

	};
}

#endif