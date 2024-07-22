#ifndef __RAINDROP_GRAPHICS_BUFFER_HPP__
#define __RAINDROP_GRAPHICS_BUFFER_HPP__

#include "types.hpp"
#include "pch.pch"

namespace Raindrop::Graphics{
	static constexpr std::size_t WHOLE_SIZE = static_cast<std::size_t>(~0ULL);
	class Buffer{
		public:
			Buffer() noexcept;
			~Buffer();

			Buffer(const Buffer&) = delete;
			Buffer& operator=(const Buffer&) = delete;

			Buffer(Buffer&& other);
			Buffer& operator=(Buffer&& other);

			friend void swap(Buffer& A, Buffer& B);

			void prepare(Context& context);
			void release();

			void allocate();
			void free();

			Buffer& setSize(const std::size_t& size);
			Buffer& setAlignement(const std::size_t& alignement);
			Buffer& setUsage(const VkBufferUsageFlags& usage);
			Buffer& setCreateFlags(const VkBufferCreateFlags& flags);
			Buffer& setMemoryProperties(const VkMemoryPropertyFlags& properties);
			Buffer& setQueueFamilies(const std::vector<uint32_t>& families);
			Buffer& setSharingMode(const VkSharingMode& sharingMode);

			const VkBuffer& get() const noexcept;
			const VkDeviceMemory& getMemory() const noexcept;

			void map(const std::size_t& size = WHOLE_SIZE, const std::size_t& offset = 0);
			void unmap();

			bool isMapped() const noexcept;
			void* getMappedPointer() noexcept;

			void flush(const std::size_t& size = WHOLE_SIZE, const std::size_t& offset = 0);
			void invalidate(const std::size_t& size = WHOLE_SIZE, const std::size_t& offset = 0);

		private:
			struct BuildInfo{
				std::size_t size;
				std::size_t alignement;
				VkBufferUsageFlags usage;
				VkBufferCreateFlags createFlags;
				VkMemoryPropertyFlags memoryPropertiess;
				std::vector<uint32_t> queueFamilies;
				VkSharingMode sharingMode;

				BuildInfo();
			};

			Context* _context;
			VkBuffer _buffer;
			VkDeviceMemory _memory;
			void* _mapped;

			std::unique_ptr<BuildInfo> _buildInfo;
			std::size_t _size;
	};
}

#endif