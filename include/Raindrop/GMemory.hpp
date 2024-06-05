#ifndef __RAINDROP_MEMORY_HPP__
#define __RAINDROP_MEMORY_HPP__

#include "common.hpp"
#include "Utils/Flags.hpp"

namespace Raindrop{
	class GMemory{
		public:
			class Type{
				public:
					struct Impl;

					class Flags : public Utils::FlagsTemplate<Flags> {
						public:
							using FlagsTemplate<Flags>::FlagsTemplate;

							enum Bits{
								NONE = 0,
								GPU_LOCAL = 1 << 0,
								CPU_VISIBLE = 1 << 1,
								CPU_COHERENT = 1 << 2,
								CPU_CACHED = 1 << 3,
								LAZY_ALLOCATION = 1 << 4,
								PROTECTED = 1 << 5,
								GPU_COHERENT = 1 << 6,
								GPU_UNCACHED = 1 << 7,
							};
					};

					static Type Find(Context& context, const Flags& required);

					Type(Context& context, const std::size_t& index);
					Type(const Type& other);
					Type(Type&& other);

					~Type();

					Type& operator=(const Type& other);
					Type& operator=(Type&& other);

					bool isInitialized() const noexcept;
					Impl* getImpl() const noexcept;

					const std::size_t& getIndex() const noexcept;
					Flags getFlags() const noexcept;

				private:
					Impl* _impl;
			};

			struct Impl;

			static GMemory Allocate(Context& context, const std::size_t& size, const Type& memoryType);

			GMemory(Context& context);
			GMemory(const GMemory& other);
			GMemory(GMemory&& other);
			~GMemory();

			GMemory& operator=(const GMemory& other);
			GMemory& operator=(GMemory&& other);

			bool isInitialized() const noexcept;
			void* getNativeHandle() const;
			Impl* getImpl() const noexcept;
			GUID getGUID() const noexcept;

			void allocate(const std::size_t& size, const Type& memoryType);
			void free();

		private:
			Impl* _impl;
	};

	inline static GMemory::Type FindGetGMemoryType(Context& context, const GMemory::Type::Flags& flags){
		return GMemory::Type::Find(context, flags);
	}

	inline static GMemory AllocateGMemory(Context& context, const std::size_t& size, const GMemory::Type& memoryType){
		return GMemory::Allocate(context, size, memoryType);
	}
}

#endif