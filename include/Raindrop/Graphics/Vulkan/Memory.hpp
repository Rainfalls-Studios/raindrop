#ifndef __RAINDROP_GRAPHICS_MEMORY_HPP__
#define __RAINDROP_GRAPHICS_MEMORY_HPP__

#include "types.hpp"
#include "pch.pch"
#include <Raindrop/Object.hpp>

namespace Raindrop::Graphics{
	class Memory : public Object{
		public:
			static std::shared_ptr<Memory> create(Raindrop::Context& context);

			Memory() noexcept;
			~Memory();

			Memory(const Memory&) = delete;
			Memory& operator=(const Memory&) = delete;

			Memory(Memory&& other);
			Memory& operator=(Memory&& other);

			friend void swap(Memory& A, Memory& B);


			void prepare(Context& context);
			void initialize();
			void release();

			Memory& setSize(const std::size_t& size);
			Memory& setMemoryIndex(const std::uint32_t& index);

			const VkDeviceMemory& get() const noexcept;

			void map(const std::size_t& size = WHOLE_SIZE, const std::size_t& offset = 0);
			void unmap();

			void* getMappedPointer();

			void flush(const std::size_t& size = WHOLE_SIZE, const std::size_t& offset = 0);
			void invalidate(const std::size_t& size = WHOLE_SIZE, const std::size_t& offset = 0);

		private:
			struct BuildInfo{
				std::size_t size;
				std::uint32_t memoryIndex;
			};

			Context* _context;
			VkDeviceMemory _memory;
			void* _mapped;

			std::unique_ptr<BuildInfo> _info;
			BuildInfo& getInfo();
	};
}

#endif