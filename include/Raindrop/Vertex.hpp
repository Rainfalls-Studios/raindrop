#ifndef __RAINDROP_VERTEX_HPP__
#define __RAINDROP_VERTEX_HPP__

#include "common.hpp"
#include "Format.hpp"
#include "GBuffer.hpp"

namespace Raindrop{
	namespace Vertex{
		enum class InputRate{
			PER_INSTANCE,
			PER_VERTEX
		};

		class Layout{
			public:
				struct Attribute{
					static constexpr std::size_t OFFSET_AUTO = static_cast<std::size_t>(~0ULL);
					static constexpr std::size_t SIZE_AUTO = static_cast<std::size_t>(~0ULL);

					Format format = Format::UNDEFINED;
					std::size_t offset = OFFSET_AUTO;
					std::size_t size = SIZE_AUTO;
					std::string name;

					Attribute(const std::string& name, const Format& format, std::size_t size = SIZE_AUTO, std::size_t offset = OFFSET_AUTO);
				};

				Layout();

				Layout& addAttribute(Attribute attribute);
				Layout& setStride(const std::size_t stride);


				const std::vector<Attribute>& getAttributes() const noexcept;
				const std::size_t& getStride() const noexcept;

			private:
				std::vector<Attribute> _attributes;
				std::size_t _stride;
				bool _isStrideCustom;
		};

		class Buffer : public GBuffer{
			public:
				Buffer(Context& context);

				const Layout& getLayout(const Layout& layout);

				void* getMappedPointer();

				void allocate(const Layout& layout, const std::size_t& vertexCount, const Usage& usage, const Flags& flags, const GMemory::Type::Flags& memoryFlags, const std::size_t& alignement = 1);

				void map(const std::size_t& vertexCount = WHOLE_SIZE, const std::size_t& vertexOffset = 0);
				void flush(const std::size_t& vertexCount = WHOLE_SIZE, const std::size_t& vertexOffset = 0);
				void invalidate(const std::size_t& vertexCount = WHOLE_SIZE, const std::size_t& vertexOffset = 0);

				const std::size_t getVertexCount() const;

			private:
				Layout _layout;
		};
	}
}

#endif