#ifndef __RAINDROP_TEXTURE_HPP__
#define __RAINDROP_TEXTURE_HPP__

#include "common.hpp"
#include "Format.hpp"
#include "Utils/Flags.hpp"

namespace Raindrop{
	class Texture{
		public:
			class Aspect : public Utils::FlagsTemplate<Aspect> {
				public:
					using FlagsTemplate<Aspect>::FlagsTemplate;

					enum Bits : Bitset {
						NONE = 0,
						COLOR = 1 << 0,
						DEPTH = 1 << 1,
						STENCIL = 1 << 2,
						METADATA = 1 << 3,
						PLANE_0 = 1 << 4,
						PLANE_1 = 1 << 5,
						PLANE_2 = 1 << 6,
						MEMORY_PLANE_0 = 1 << 7,
						MEMORY_PLANE_1 = 1 << 8,
						MEMORY_PLANE_2 = 1 << 9,
						MEMORY_PLANE_3 = 1 << 10
					};
			};

			enum class Tiling{
				OPTIMAL,
				LINEAR
			};

			enum class Layout{
				UNDEFINED,
				GENERAL,
				COLOR_ATTACHMENT_OPTIMAL,
				DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
				DEPTH_STENCIL_READ_ONLY_OPTIMAL,
				SHADER_READ_ONLY_OPTIMAL,
				TRANSFER_SRC_OPTIMAL,
				TRANSFER_DST_OPTIMAL,
				PREINITIALIZED,
				DEPTH_READ_ONLY_STENCIL_ATTACHMENT_OPTIMAL,
				DEPTH_ATTACHMENT_STENCIL_READ_ONLY_OPTIMAL,
				DEPTH_ATTACHMENT_OPTIMAL,
				DEPTH_READ_ONLY_OPTIMAL,
				STENCIL_ATTACHMENT_OPTIMAL,
				STENCIL_READ_ONLY_OPTIMAL,
				READ_ONLY_OPTIMAL,
				ATTACHMENT_OPTIMAL,
				PRESENT_SRC,
				VIDEO_DECODE_DST,
				VIDEO_DECODE_SRC,
				VIDEO_DECODE_DPB,
				SHARED_PRESENT,
				FRAGMENT_DENSITY_MAP_OPTIMAL,
				FRAGMENT_SHADING_RATE_ATTACHMENT_OPTIMAL,
				RENDERING_LOCAL_READ,
				VIDEO_ENCODE_DST,
				VIDEO_ENCODE_SRC,
				VIDEO_ENCODE_DPB,
				ATTACHMENT_FEEDBACK_LOOP_OPTIMAL
			};

			enum class Type{
				LINEAR,
				PLANAR,
				VOLUMETRIC
			};

			class Usage : public Utils::FlagsTemplate<Usage> {
				public:
					using FlagsTemplate<Usage>::FlagsTemplate;

					enum Bits : Bitset {
						NONE = 0,
						TRANSFER_SRC = 1 << 0,
						TRANSFER_DST = 1 << 1,
						SAMPLED = 1 << 2,
						STORAGE = 1 << 3,
						COLOR_ATTACHMENT = 1 << 4,
						DEPTH_STENCIL_ATTACHMENT = 1 << 5,
						TRANSIENT_ATTACHMENT = 1 << 6,
						INPUT_ATTACHMENT = 1 << 7,
						VIDEO_DECODE_DST = 1 << 8,
						VIDEO_DECODE_SRC = 1 << 9,
						VIDEO_DECODE_DPB = 1 << 10,
						FRAGMENT_DENSITY_MAP = 1 << 11,
						FRAGMENT_SHADING_RATE_ATTACHMENT = 1 << 12,
						HOST_TRANSFER = 1 << 13,
						VIDEO_ENCODE_DST = 1 << 14,
						VIDEO_ENCODE_SRC = 1 << 15,
						VIDEO_ENCODE_DPB = 1 << 16,
						ATTACHMENT_FEEDBACK_LOOP = 1 << 17,
						INVOCATION_MASK = 1 << 18,
						SAMPLE_WEIGHT = 1 << 19,
						SAMPLE_BLOCK_MATCH = 1 << 20,

						SHADING_RATE_IMAGE = FRAGMENT_SHADING_RATE_ATTACHMENT
					};
			};

			class Flags : public Utils::FlagsTemplate<Flags> {
				public:
					using FlagsTemplate<Flags>::FlagsTemplate;

					enum Bits : Bitset {
						NONE = 0,
						SPARSE_BINDING = 1 << 0,
						SPARSE_RESIDENCY = 1 << 1,
						SPARSE_ALIASED = 1 << 2,
						MUTABLE_FORMAT = 1 << 3,
						CUBE_COMPATIBLE = 1 << 4,
						ALIAS = 1 << 5,
						SPLIT_INSTANCE_BIND_REGIONS = 1 << 6,
						PLANAR_ARRAY_COMPATIBLE = 1 << 7,
						BLOCK_TEXEL_VIEW_COMPATIBLE = 1 << 8,
						EXTENDED_USAGE = 1 << 9,
						PROTECTED = 1 << 10,
						DISJOINT = 1 << 11,
						CORNER_SAMPLED = 1 << 12,
						SAMPLE_LOCATIONS_COMPATIBLE_DEPTH = 1 << 13,
						SUBSAMPLED = 1 << 14,
						DESCRIPTOR_BUFFER_CAPTURE_REPLAY = 1 << 15,
						MULTISAMPLED_RENDER_TO_SINGLE_SAMPLED = 1 << 16,
						PLANAR_VIEW_COMPATIBLE = 1 << 17,
						FRAGMENT_DENSITY_MAP_OFFSET = 1 << 18,
						VIDEO_PROFILE_INDEPENDENT = 1 << 19,
					};
			};

			struct FormatProperties{
				bool supported;
				std::uint32_t maxWidth;
				std::uint32_t maxHeight;
				std::uint32_t maxDepth;
				std::uint32_t maxMipmapLevels;
				std::uint32_t maxArrayLayers;
				// Texture::Sample supportedSampleCount;
				std::uint32_t maxResourcesSize;

				static const FormatProperties UNSUPPORTED;
			};

			struct Impl;

			static Texture Create(Context& context);
			
			Texture(Context& context);
			Texture(std::unique_ptr<Impl>&& impl);
			~Texture();

			void initialize();
			void release();

			void setFormat(const Format& format);
			void setWidth(const std::uint32_t& width);
			void setHeight(const std::uint32_t& height);
			void setDepth(const std::uint32_t& depth);
			void setUsage(const Usage& usage);
			void setLayout(const Layout& layout);
			void setTiling(const Tiling& tiling);
			void setType(const Type& type);
			void setMipmapCount(const std::uint32_t mip);
			void setArrayLayers(const std::uint32_t layers);
			void setFlags(const Flags& flags);

			Format getFormat() const noexcept;
			std::uint32_t getWidth() const noexcept;
			std::uint32_t getHeight() const noexcept;
			std::uint32_t getDepth() const noexcept;
			Usage getUsage() const noexcept;
			Layout getLayout() const noexcept;
			Tiling getTiling() const noexcept;
			Type getType() const noexcept;
			std::uint32_t getMipmapCount() const noexcept;
			std::uint32_t getArrayLayers() const noexcept;
			Flags getFlags() const noexcept;

			bool isInitialized() const noexcept;
			void* getNativeHandle() const;
			Impl* getImpl() const noexcept;
			GUID getGUID() const noexcept;

			FormatProperties getFormatProperties(const Format& format) const;
			std::list<Format> findAllSupportedFormats(const Format::Properties& requiredProperties, const Format::Features& requiredFeatures, const Format::Properties& except = Format::Properties::NONE) const;
			Format findBestFormat(const Format::Properties& requiredProperties, const Format::Features& requiredFeatures, const Format::Properties& except = Format::Properties::NONE) const;

		private:
			std::unique_ptr<Impl> _impl;
	};

	inline static Texture CreateTexture(Context& context){
		return Texture::Create(context);
	}
}

#endif