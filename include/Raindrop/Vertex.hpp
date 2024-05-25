#ifndef __RAINDROP_VERTEX_HPP__
#define __RAINDROP_VERTEX_HPP__

#include "common.hpp"
#include "Format.hpp"

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

		// TODO maybe
		// class Buffer;
	}
}

#endif