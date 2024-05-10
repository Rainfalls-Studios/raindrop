#ifndef __RAINDROP_TEXTURE_SUBSET_HPP__
#define __RAINDROP_TEXTURE_SUBSET_HPP__

#include "common.hpp"
#include "Format.hpp"
#include "Texture.hpp"
#include "Color.hpp"
#include "Utils/Flags.hpp"

namespace Raindrop{
	class TextureSubset{
		public:
			struct Range{
				Texture::Aspect aspect = Texture::Aspect::NONE;
				std::size_t mipmapLevel = 0;
				std::size_t mipmapCount = 1;
				std::size_t layer = 0;
				std::size_t layerCount = 1;
			};

			enum class Type{
				LINEAR,
				PLANAR,
				VOLUMETRIC,
				CUBEMAP,
				LINEAR_ARRAY,
				PLANAR_ARRAY,
				CUBEMAP_ARRAY
			};

			class Flags : public Utils::FlagsTemplate<Flags> {
				public:
					using FlagsTemplate<Flags>::FlagsTemplate;

					enum Bits : Bitset {
						NONE = 0,
						FRAGMENT_DENSITY_MAP_DYNAMIC = 1 << 1,
						FRAGMENT_DENSITY_MAP_DEFERRED = 1 << 2,
						DESCRIPTOR_BUFFER_CAPTURE_REPLAY = 1 << 3
					};
			};

			using Handle = void*;

			static TextureSubset Create(Context& context, const Texture& origin);

			TextureSubset();

			TextureSubset(const TextureSubset& other);
			TextureSubset& operator=(const TextureSubset& other);

			void initialize();
			void release();

			bool isInitialized() const noexcept;
			void* getNativeHandle() const noexcept;
			Handle getHandle() const noexcept;

			void setSource(const Texture& source);
			void setRange(const Range& range);
			void swizzleComponent(const Color::Component& component, const Color::Swizzle& swizzle);
			void setType(const Type& type);
			void setFormat(const Format& format);
			void setFlags(const Flags& info);

			const Texture& getSource() const noexcept;
			const Range& getRange() const noexcept;
			const Color::Swizzle& getComponentSwizzle(const Color::Component& component) const noexcept;
			const Type& getType() const noexcept;
			const Format& getFormat() const noexcept;
			const Flags& getFlags() const noexcept;

		private:
			Handle _handle;
	};

	static inline TextureSubset CreateTextureSubset(Context& context, const Texture& origin){
		return TextureSubset::Create(context, origin);
	}
}

#endif