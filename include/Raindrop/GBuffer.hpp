#ifndef __RAINDROP_GBUFFER_HPP__
#define __RAINDROP_GBUFFER_HPP__

#include "common.hpp"
#include "Utils/Flags.hpp"
#include "GMemory.hpp"

namespace Raindrop{
	static constexpr std::size_t WHOLE_SIZE = static_cast<std::size_t>(~0ULL);
	class GBuffer{
		public:
			struct Impl;

			class Usage : public Utils::FlagsTemplate<Usage> {
				public:
					using FlagsTemplate<Usage>::FlagsTemplate;

					enum Bits{
						NONE = 0,
						TRANSFER_SRC = 1 << 0,
						TRANSFER_DST = 1 << 1,
						UNIFORM_TEXEL_BUFFER = 1 << 2,
						STORAGE_TEXEL_BUFFER = 1 << 3,
						UNIFORM_BUFFER = 1 << 4,
						STORAGE_BUFFER = 1 << 5,
						INDEX_BUFFER = 1 << 6,
						VERTEX_BUFFER = 1 << 7,
						INDIRECT_BUFFER = 1 << 8,
						SHADER_DEVICE_ADDRESS = 1 << 9,
						VIDEO_DECODE_SRC = 1 << 10,
						VIDEO_DECODE_DST = 1 << 11,
						TRANSFORM_FEEDBACK_BUFFER = 1 << 12,
						TRANSFORM_FEEDBACK_COUNTER_BUFFER = 1 << 13,
						CONDITIONAL_RENDERING = 1 << 14,
						ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY = 1 << 15,
						ACCELERATION_STRUCTURE_STORAGE = 1 << 16,
						SHADER_BINDING_TABLE = 1 << 17,
						VIDEO_ENCODE_DST = 1 << 18,
						VIDEO_ENCODE_SRC = 1 << 19,
						SAMPLER_DESCRIPTOR_BUFFER = 1 << 20,
						RESOURCE_DESCRIPTOR_BUFFER = 1 << 21,
						PUSH_DESCRIPTORS_DESCRIPTOR_BUFFER = 1 << 22,
						MICROMAP_BUILD_INPUT_READ_ONLY = 1 << 23,
						MICROMAP_STORAGE = 1 << 24,
					};
			};

			class Flags : public Utils::FlagsTemplate<Flags> {
				public:
					using FlagsTemplate<Flags>::FlagsTemplate;

					enum Bits{
						NONE = 0,
						SPARSE_BINDING = 1 << 0,
						SPARSE_RESIDENCY = 1 << 1,
						SPARSE_ALIASED = 1 << 2,
						PROTECTED = 1 << 3,
						DEVICE_ADDRESS_CAPTURE_REPLAY = 1 << 4,
						DESCRIPTOR_BUFFER_CAPTURE_REPLAY = 1 << 5,
						VIDEO_PROFILE_INDEPENDENT = 1 << 6,
					};
			};

			static GBuffer Create(Context& context);

			GBuffer(Context& context);
			~GBuffer();

			bool isInitialized() const noexcept;
			void* getNativeHandle() const;
			Impl* getImpl() const noexcept;
			GUID getGUID() const noexcept;

			void allocate(const std::size_t& size, const Usage& usage, const Flags& flags, const GMemory::Type::Flags& memoryFlags, const std::size_t& alignement = 1);
			void free();

			void map(const std::size_t& size = WHOLE_SIZE, const std::size_t& offset = 0);
			void unmap();
			void* getMappedPointer();

			void flush(const std::size_t& size = WHOLE_SIZE, const std::size_t& offset = 0);
			void invalidate(const std::size_t& size = WHOLE_SIZE, const std::size_t& offset = 0);

			const std::size_t& getSize() const noexcept;
			const std::size_t& getAllignement() const noexcept;
			Usage getUsage() const noexcept;
			Flags getFlags() const noexcept;
			GMemory::Type::Flags getMemoryFlags() const noexcept;

		private:
			std::unique_ptr<Impl> _impl;
	};

	inline static GBuffer CreateGBuffer(Context& context){
		return GBuffer::Create(context);
	}

}

#endif