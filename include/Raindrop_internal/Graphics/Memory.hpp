#ifndef __RAINDROP_INTERNAL_GRAPHICS_MEMORY_HPP__
#define __RAINDROP_INTERNAL_GRAPHICS_MEMORY_HPP__

#include "common.hpp"

namespace Raindrop::Internal::Graphics{
	class Memory{
		public:
			~Memory();

			Memory(Context& context, MemoryType& type, const std::size_t& size);
			Memory(const Memory&) = delete;
			Memory& operator=(const Memory&) = delete;

			Memory(Memory&& other) noexcept;
			Memory& operator=(Memory&&) = delete;

			VkDeviceMemory get() const noexcept;
			std::size_t getSize() const noexcept;

			const MemoryType& getType() const noexcept;

		private:
			Memory(Context& context, MemoryType& type);

			Context& _context;

			VkDeviceMemory _memory;
			std::size_t _size;
			MemoryType& _type;
	};

	class MemoryType{
		public:
			MemoryType(const std::size_t& index, const VkMemoryType& info);
			~MemoryType();

			const std::size_t& getIndex() const noexcept;
			const VkMemoryPropertyFlags& getFlags() const noexcept;

			void notifyAllocation(const std::size_t& size) noexcept;
			void notifyFree(const std::size_t& size) noexcept;

		private:
			std::size_t _index;
			VkMemoryPropertyFlags _flags;
	};

	class MemoryTypes{
		public:
			MemoryTypes(Context& context);
			~MemoryTypes();

			const std::vector<MemoryType>& getTypes() const noexcept;
			std::vector<MemoryType>& getTypes() noexcept;

			MemoryType& get(const std::size_t& index);
			const MemoryType& get(const std::size_t& index) const;

			MemoryType& findBest(const VkMemoryPropertyFlags& flags);

		private:
			Context& _context;
			std::vector<MemoryType> _types;
	};
}

#endif