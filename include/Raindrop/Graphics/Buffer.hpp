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

			void prepare(Context& context);
			void release();

			void allocate(const std::size_t& size, const VkBufferUsageFlags& usage, const VkBufferCreateFlags& flags, const VkMemoryPropertyFlags& memoryFlags, const std::size_t& alignement = 1);
			void free();

			const VkBuffer& get() const noexcept;
			const VkDeviceMemory& getMemory() const noexcept;

			void map(const std::size_t& size = WHOLE_SIZE, const std::size_t& offset = 0);
			void unmap();

			bool isMapped() const noexcept;
			void* getMappedPointer() noexcept;

			void flush(const std::size_t& size = WHOLE_SIZE, const std::size_t& offset = 0);
			void invalidate(const std::size_t& size = WHOLE_SIZE, const std::size_t& offset = 0);

		private:
			Context* _context;
			VkBuffer _buffer;
			VkDeviceMemory _memory;
			void* _mapped;


			std::size_t _size;
	};
}

#endif