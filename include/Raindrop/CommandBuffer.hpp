#ifndef __RAINDROP_COMMAND_BUFFER_HPP__
#define __RAINDROP_COMMAND_BUFFER_HPP__

#include "common.hpp"
#include "Utils/Flags.hpp"

namespace Raindrop{
	class CommandBuffer{
		friend class Pool;
		public:
			struct Impl;

			class Flags : public Utils::FlagsTemplate<Flags> {
				public:
					using FlagsTemplate<Flags>::FlagsTemplate;

					enum Bits{
						NONE = 0,
						SINGLE_SUBMIT = 1 << 0,
						RENDER_PASS_CONTINUE = 1 << 1,
						SIMULTANEOUS_USE = 1 << 2,
					};
			};

			class Usage : public Utils::FlagsTemplate<Usage> {
				public:
					using FlagsTemplate<Usage>::FlagsTemplate;

					enum Bits{
						NONE = 0,
						GRAPHICS = 1 << 0,
						COMPUTE = 1 << 1,
						TRANSFER = 1 << 2,
						SPARSE_BINDING = 1 << 3,
						PROTECTED = 1 << 4,
						VIDEO_DECODE = 1 << 5,
						VIDEO_ENCODE = 1 << 6,
						OPTICAL_FLOW = 1 << 7
					};
			};

			class AllocationFlags : public Utils::FlagsTemplate<AllocationFlags> {
				public:
					using FlagsTemplate<AllocationFlags>::FlagsTemplate;

					enum Bits{
						NONE = 0,
						SECONDARY = 1 << 0,
					};
			};

			class Pool{
				public:
					struct Impl;

					class Flags : public Utils::FlagsTemplate<Flags> {
						public:
							using FlagsTemplate<Flags>::FlagsTemplate;

							enum Bits{
								NONE = 0,
								SHORT_LIVED = 1 << 0,
								INDEPENDENT_RESET = 1 << 1,
								PROTECTED = 1 << 2
							};
					};

					Pool(Context& context, const Usage& usage, const Flags& flags);
					~Pool();

					CommandBuffer allocate(const CommandBuffer::AllocationFlags& flags);
					std::vector<CommandBuffer> allocate(const uint32_t& count, const CommandBuffer::AllocationFlags& flags);

					void free(const CommandBuffer& commandBuffer);
					void free(const std::vector<CommandBuffer>& commandBuffers);

					void reset(const bool& release = false);

				private:
					Impl* _impl;

			};

			~CommandBuffer();
			CommandBuffer(Impl* impl);

			CommandBuffer(const CommandBuffer& other) = delete;
			CommandBuffer& operator=(const CommandBuffer& other) = delete;

			CommandBuffer(CommandBuffer&& other) noexcept;
			CommandBuffer& operator=(CommandBuffer&& other) noexcept;
			

			void begin(const Flags& flags = Flags::NONE);
			void end();

		private:
			Impl* _impl;

	};
}

#endif