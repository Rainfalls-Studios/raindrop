#ifndef __RAINDROP_FORMAT_HPP__
#define __RAINDROP_FORMAT_HPP__

#include "common.hpp"
#include "Utils/Flags.hpp"

namespace Raindrop{
	class Format{
		public:
			// Based out of VkFormat
			enum Type{
				UNDEFINED,
				R4G4_UNORM_PACK8,
				R4G4B4A4_UNORM_PACK16,
				B4G4R4A4_UNORM_PACK16,
				R5G6B5_UNORM_PACK16,
				B5G6R5_UNORM_PACK16,
				R5G5B5A1_UNORM_PACK16,
				B5G5R5A1_UNORM_PACK16,
				A1R5G5B5_UNORM_PACK16,
				R8_UNORM,
				R8_SNORM,
				R8_USCALED,
				R8_SSCALED,
				R8_UINT,
				R8_SINT,
				R8_SRGB,
				R8G8_UNORM,
				R8G8_SNORM,
				R8G8_USCALED,
				R8G8_SSCALED,
				R8G8_UINT,
				R8G8_SINT,
				R8G8_SRGB,
				R8G8B8_UNORM,
				R8G8B8_SNORM,
				R8G8B8_USCALED,
				R8G8B8_SSCALED,
				R8G8B8_UINT,
				R8G8B8_SINT,
				R8G8B8_SRGB,
				B8G8R8_UNORM,
				B8G8R8_SNORM,
				B8G8R8_USCALED,
				B8G8R8_SSCALED,
				B8G8R8_UINT,
				B8G8R8_SINT,
				B8G8R8_SRGB,
				R8G8B8A8_UNORM,
				R8G8B8A8_SNORM,
				R8G8B8A8_USCALED,
				R8G8B8A8_SSCALED,
				R8G8B8A8_UINT,
				R8G8B8A8_SINT,
				R8G8B8A8_SRGB,
				B8G8R8A8_UNORM,
				B8G8R8A8_SNORM,
				B8G8R8A8_USCALED,
				B8G8R8A8_SSCALED,
				B8G8R8A8_UINT,
				B8G8R8A8_SINT,
				B8G8R8A8_SRGB,
				A8B8G8R8_UNORM_PACK32,
				A8B8G8R8_SNORM_PACK32,
				A8B8G8R8_USCALED_PACK32,
				A8B8G8R8_SSCALED_PACK32,
				A8B8G8R8_UINT_PACK32,
				A8B8G8R8_SINT_PACK32,
				A8B8G8R8_SRGB_PACK32,
				A2R10G10B10_UNORM_PACK32,
				A2R10G10B10_SNORM_PACK32,
				A2R10G10B10_USCALED_PACK32,
				A2R10G10B10_SSCALED_PACK32,
				A2R10G10B10_UINT_PACK32,
				A2R10G10B10_SINT_PACK32,
				A2B10G10R10_UNORM_PACK32,
				A2B10G10R10_SNORM_PACK32,
				A2B10G10R10_USCALED_PACK32,
				A2B10G10R10_SSCALED_PACK32,
				A2B10G10R10_UINT_PACK32,
				A2B10G10R10_SINT_PACK32,
				R16_UNORM,
				R16_SNORM,
				R16_USCALED,
				R16_SSCALED,
				R16_UINT,
				R16_SINT,
				R16_SFLOAT,
				R16G16_UNORM,
				R16G16_SNORM,
				R16G16_USCALED,
				R16G16_SSCALED,
				R16G16_UINT,
				R16G16_SINT,
				R16G16_SFLOAT,
				R16G16B16_UNORM,
				R16G16B16_SNORM,
				R16G16B16_USCALED,
				R16G16B16_SSCALED,
				R16G16B16_UINT,
				R16G16B16_SINT,
				R16G16B16_SFLOAT,
				R16G16B16A16_UNORM,
				R16G16B16A16_SNORM,
				R16G16B16A16_USCALED,
				R16G16B16A16_SSCALED,
				R16G16B16A16_UINT,
				R16G16B16A16_SINT,
				R16G16B16A16_SFLOAT,
				R32_UINT,
				R32_SINT,
				R32_SFLOAT,
				R32G32_UINT,
				R32G32_SINT,
				R32G32_SFLOAT,
				R32G32B32_UINT,
				R32G32B32_SINT,
				R32G32B32_SFLOAT,
				R32G32B32A32_UINT,
				R32G32B32A32_SINT,
				R32G32B32A32_SFLOAT,
				R64_UINT,
				R64_SINT,
				R64_SFLOAT,
				R64G64_UINT,
				R64G64_SINT,
				R64G64_SFLOAT,
				R64G64B64_UINT,
				R64G64B64_SINT,
				R64G64B64_SFLOAT,
				R64G64B64A64_UINT,
				R64G64B64A64_SINT,
				R64G64B64A64_SFLOAT,
				B10G11R11_UFLOAT_PACK32,
				E5B9G9R9_UFLOAT_PACK32,
				D16_UNORM,
				X8_D24_UNORM_PACK32,
				D32_SFLOAT,
				S8_UINT,
				D16_UNORM_S8_UINT,
				D24_UNORM_S8_UINT,
				D32_SFLOAT_S8_UINT,
				BC1_RGB_UNORM_BLOCK,
				BC1_RGB_SRGB_BLOCK,
				BC1_RGBA_UNORM_BLOCK,
				BC1_RGBA_SRGB_BLOCK,
				BC2_UNORM_BLOCK,
				BC2_SRGB_BLOCK,
				BC3_UNORM_BLOCK,
				BC3_SRGB_BLOCK,
				BC4_UNORM_BLOCK,
				BC4_SNORM_BLOCK,
				BC5_UNORM_BLOCK,
				BC5_SNORM_BLOCK,
				BC6H_UFLOAT_BLOCK,
				BC6H_SFLOAT_BLOCK,
				BC7_UNORM_BLOCK,
				BC7_SRGB_BLOCK,
				ETC2_R8G8B8_UNORM_BLOCK,
				ETC2_R8G8B8_SRGB_BLOCK,
				ETC2_R8G8B8A1_UNORM_BLOCK,
				ETC2_R8G8B8A1_SRGB_BLOCK,
				ETC2_R8G8B8A8_UNORM_BLOCK,
				ETC2_R8G8B8A8_SRGB_BLOCK,
				EAC_R11_UNORM_BLOCK,
				EAC_R11_SNORM_BLOCK,
				EAC_R11G11_UNORM_BLOCK,
				EAC_R11G11_SNORM_BLOCK,
				ASTC_4x4_UNORM_BLOCK,
				ASTC_4x4_SRGB_BLOCK,
				ASTC_5x4_UNORM_BLOCK,
				ASTC_5x4_SRGB_BLOCK,
				ASTC_5x5_UNORM_BLOCK,
				ASTC_5x5_SRGB_BLOCK,
				ASTC_6x5_UNORM_BLOCK,
				ASTC_6x5_SRGB_BLOCK,
				ASTC_6x6_UNORM_BLOCK,
				ASTC_6x6_SRGB_BLOCK,
				ASTC_8x5_UNORM_BLOCK,
				ASTC_8x5_SRGB_BLOCK,
				ASTC_8x6_UNORM_BLOCK,
				ASTC_8x6_SRGB_BLOCK,
				ASTC_8x8_UNORM_BLOCK,
				ASTC_8x8_SRGB_BLOCK,
				ASTC_10x5_UNORM_BLOCK,
				ASTC_10x5_SRGB_BLOCK,
				ASTC_10x6_UNORM_BLOCK,
				ASTC_10x6_SRGB_BLOCK,
				ASTC_10x8_UNORM_BLOCK,
				ASTC_10x8_SRGB_BLOCK,
				ASTC_10x10_UNORM_BLOCK,
				ASTC_10x10_SRGB_BLOCK,
				ASTC_12x10_UNORM_BLOCK,
				ASTC_12x10_SRGB_BLOCK,
				ASTC_12x12_UNORM_BLOCK,
				ASTC_12x12_SRGB_BLOCK,
				G8B8G8R8_422_UNORM,
				B8G8R8G8_422_UNORM,
				G8_B8_R8_3PLANE_420_UNORM,
				G8_B8R8_2PLANE_420_UNORM,
				G8_B8_R8_3PLANE_422_UNORM,
				G8_B8R8_2PLANE_422_UNORM,
				G8_B8_R8_3PLANE_444_UNORM,
				R10X6_UNORM_PACK16,
				R10X6G10X6_UNORM_2PACK16,
				R10X6G10X6B10X6A10X6_UNORM_4PACK16,
				G10X6B10X6G10X6R10X6_422_UNORM_4PACK16,
				B10X6G10X6R10X6G10X6_422_UNORM_4PACK16,
				G10X6_B10X6_R10X6_3PLANE_420_UNORM_3PACK16,
				G10X6_B10X6R10X6_2PLANE_420_UNORM_3PACK16,
				G10X6_B10X6_R10X6_3PLANE_422_UNORM_3PACK16,
				G10X6_B10X6R10X6_2PLANE_422_UNORM_3PACK16,
				G10X6_B10X6_R10X6_3PLANE_444_UNORM_3PACK16,
				R12X4_UNORM_PACK16,
				R12X4G12X4_UNORM_2PACK16,
				R12X4G12X4B12X4A12X4_UNORM_4PACK16,
				G12X4B12X4G12X4R12X4_422_UNORM_4PACK16,
				B12X4G12X4R12X4G12X4_422_UNORM_4PACK16,
				G12X4_B12X4_R12X4_3PLANE_420_UNORM_3PACK16,
				G12X4_B12X4R12X4_2PLANE_420_UNORM_3PACK16,
				G12X4_B12X4_R12X4_3PLANE_422_UNORM_3PACK16,
				G12X4_B12X4R12X4_2PLANE_422_UNORM_3PACK16,
				G12X4_B12X4_R12X4_3PLANE_444_UNORM_3PACK16,
				G16B16G16R16_422_UNORM,
				B16G16R16G16_422_UNORM,
				G16_B16_R16_3PLANE_420_UNORM,
				G16_B16R16_2PLANE_420_UNORM,
				G16_B16_R16_3PLANE_422_UNORM,
				G16_B16R16_2PLANE_422_UNORM,
				G16_B16_R16_3PLANE_444_UNORM,
				G8_B8R8_2PLANE_444_UNORM,
				G10X6_B10X6R10X6_2PLANE_444_UNORM_3PACK16,
				G12X4_B12X4R12X4_2PLANE_444_UNORM_3PACK16,
				G16_B16R16_2PLANE_444_UNORM,
				A4R4G4B4_UNORM_PACK16,
				A4B4G4R4_UNORM_PACK16,
				ASTC_4x4_SFLOAT_BLOCK,
				ASTC_5x4_SFLOAT_BLOCK,
				ASTC_5x5_SFLOAT_BLOCK,
				ASTC_6x5_SFLOAT_BLOCK,
				ASTC_6x6_SFLOAT_BLOCK,
				ASTC_8x5_SFLOAT_BLOCK,
				ASTC_8x6_SFLOAT_BLOCK,
				ASTC_8x8_SFLOAT_BLOCK,
				ASTC_10x5_SFLOAT_BLOCK,
				ASTC_10x6_SFLOAT_BLOCK,
				ASTC_10x8_SFLOAT_BLOCK,
				ASTC_10x10_SFLOAT_BLOCK,
				ASTC_12x10_SFLOAT_BLOCK,
				ASTC_12x12_SFLOAT_BLOCK,
				PVRTC1_2BPP_UNORM_BLOCK_IMG,
				PVRTC1_4BPP_UNORM_BLOCK_IMG,
				PVRTC2_2BPP_UNORM_BLOCK_IMG,
				PVRTC2_4BPP_UNORM_BLOCK_IMG,
				PVRTC1_2BPP_SRGB_BLOCK_IMG,
				PVRTC1_4BPP_SRGB_BLOCK_IMG,
				PVRTC2_2BPP_SRGB_BLOCK_IMG,
				PVRTC2_4BPP_SRGB_BLOCK_IMG,
				R16G16_S10_5_NV
			};
			
			class Properties : public Utils::FlagsTemplate<Properties, std::uint64_t> {
				public:
					using FlagsTemplate<Properties, std::uint64_t>::FlagsTemplate;

					enum Bits : Bitset {
						NONE = 0,
						SFLOAT = 1ULL << 0,
						SINT = 1ULL << 1,
						SNORM = 1ULL << 2,
						SSCALED = 1ULL << 3,
						UFLOAT = 1ULL << 4,
						UINT = 1ULL << 5,
						UNORM = 1ULL << 6,
						USCALED = 1ULL << 7,
						SAMPLED_INT = 1ULL << 8,
						SAMPLED_FLOAT = 1ULL << 9,
						COMPRESSED_ASTC_HDR = 1ULL << 10,
						COMPRESSED_ASTC_LDR = 1ULL << 11,
						COMPRESSED_BC = 1ULL << 12,
						COMPRESSED_EAC = 1ULL << 13,
						COMPRESSED_ETC2 = 1ULL << 14,
						COMPRESSED_PVRTC = 1ULL << 15,
						COMPRESSED = 1ULL << 16,
						DEPTH = 1ULL << 17,
						STENCIL = 1ULL << 18,
						PACKED = 1ULL << 19,
						X_CHORMA_SUBSAMPLED = 1ULL << 20,
						Y_CHORMA_SUBSAMPLED = 1ULL << 21,
						SINGLE_PLANE_422 = 1ULL << 22,
						MULTIPLANE = 1ULL << 23,
						BIT8 = 1ULL << 24,
						BIT16 = 1ULL << 25,
						BIT32 = 1ULL << 26,
						BIT64 = 1ULL << 27,
						RED = 1ULL << 28,
						GREEN = 1ULL << 29,
						BLUE = 1ULL << 30,
						ALPHA = 1ULL << 31,
						REQUIRES_YBCBR_CONVERSION = 1ULL << 32
					};
			};

			class Features : public Utils::FlagsTemplate<Features> {
				public:
					using FlagsTemplate<Features>::FlagsTemplate;

					enum Bits : Bitset {
						NONE = 0,
						SAMPLED_IMAGE = 1ULL << 1,
						STORAGE_IMAGE = 1ULL << 2,
						STORAGE_IMAGE_ATOMIC = 1ULL << 3,
						UNIFORM_TEXEL_BUFFER = 1ULL << 4,
						STORAGE_TEXEL_BUFFER = 1ULL << 5,
						STORAGE_TEXEL_BUFFER_ATOMIC = 1ULL << 6,
						VERTEX_BUFFER = 1ULL << 7,
						COLOR_ATTACHMENT = 1ULL << 8,
						COLOR_ATTACHMENT_BLEND = 1ULL << 9,
						DEPTH_STENCIL_ATTACHMENT = 1ULL << 10,
						BLIT_SRC = 1ULL << 11,
						BLIT_DST = 1ULL << 12,
						SAMPLED_IMAGE_FILTER_LINEAR = 1ULL << 13,
						TRANSFER_SRC = 1ULL << 14,
						TRANSFER_DST = 1ULL << 15,
						MIDPOINT_CHROMA_SAMPLES = 1ULL << 16,
						SAMPLED_IMAGE_YCBCR_CONVERSION_LINEAR_FILTER = 1ULL << 17,
						SAMPLED_IMAGE_YCBCR_CONVERSION_SEPARATE_RECONSTRUCTION_FILTER = 1ULL << 18,
						SAMPLED_IMAGE_YCBCR_CONVERSION_CHROMA_RECONSTRUCTION_EXPLICIT = 1ULL << 19,
						SAMPLED_IMAGE_YCBCR_CONVERSION_CHROMA_RECONSTRUCTION_EXPLICIT_FORCEABLE = 1ULL << 20,
						DISJOINT = 1ULL << 21,
						COSITED_CHROMA_SAMPLES = 1ULL << 22,
						SAMPLED_IMAGE_FILTER_MINMAX = 1ULL << 23,
						VIDEO_DECODE_OUTPUT = 1ULL << 24,
						VIDEO_DECODE_DPB = 1ULL << 24,
						ACCELERATION_STRUCTURE_VERTEX_BUFFER = 1ULL << 16,
						SAMPLED_IMAGE_FILTER_CUBIC = 1ULL << 27,
						FRAGMENT_DENSITY_MAP = 1ULL << 28,
						FRAGMENT_SHADING_RATE_ATTACHMENT = 1ULL << 29,
						VIDEO_ENCODE_INPUT = 1ULL << 30,
						VIDEO_ENCODE_DPB = 1ULL << 31
					};
			};

			Format() noexcept;
			constexpr inline Format(Type type) noexcept : _type{type}{}
			Format(const Format& other) noexcept;

			Format& operator=(const Format& other) noexcept;
			Format& operator=(const Format::Type& type) noexcept;

			bool operator==(const Format& other) const noexcept;
			bool operator==(const Format::Type& type) const noexcept;

			bool operator!=(const Format& other) const noexcept;
			bool operator!=(const Format::Type& type) const noexcept;

			constexpr inline Type get() const noexcept{
				return _type;
			}

			bool isSFloat() const noexcept;
			bool isSInt() const noexcept;
			bool isSNorm() const noexcept;
			bool isSScaled() const noexcept;
			bool isUFloat() const noexcept;
			bool isUInt() const noexcept;
			bool isUNorm() const noexcept;
			bool isUScaled() const noexcept;
			bool isSampledInt() const noexcept;
			bool isSampledFloat() const noexcept;
			bool isCompressed_ASTC_HDR() const noexcept;
			bool isCompressed_ASTC_LDR() const noexcept;
			bool isCompressed_BC() const noexcept;
			bool isCompressed_EAC() const noexcept;
			bool isCompressed_ETC2() const noexcept;
			bool isCompressed_PVRTC() const noexcept;
			bool isCompressed() const noexcept;
			bool isPacked() const noexcept;
			bool isXChromaSubsampled() const noexcept;
			bool isYChromaSubsampled() const noexcept;
			bool isSinglePlane_422() const noexcept;
			bool isMultiplane() const noexcept;
			bool is8Bit() const noexcept;
			bool is16Bit() const noexcept;
			bool is32Bit() const noexcept;
			bool is64Bit() const noexcept;
			bool isColor() const noexcept;
			bool isDepthOnly() const noexcept;
			bool isStencilOnly() const noexcept;

			bool hasDepth() const noexcept;
			bool hasStencil() const noexcept;
			bool hasRed() const noexcept;
			bool hasGreen() const noexcept;
			bool hasBlue() const noexcept;
			bool hasAlpha() const noexcept;

			bool requiresYbcbrConversion() const noexcept;

			std::size_t getComponentCount() const noexcept;
			std::size_t getPlaneCount() const noexcept;

			std::size_t getDepthSize() const noexcept;
			std::size_t getStencilSize() const noexcept;

			std::size_t getSize() const noexcept;
			std::size_t getTexelCount() const noexcept;

			Properties getProperties() const noexcept;

			static Format FindTilingOptimal(Context& context, const Properties& requiredProperties, const Features& requiredFeatures, const Properties& except = Properties::NONE);
			static Format FindTilingLinear(Context& context, const Properties& requiredProperties, const Features& requiredFeatures, const Properties& except = Properties::NONE);
			static Format FindBuffer(Context& context, const Properties& requiredProperties, const Features& requiredFeatures, const Properties& except = Properties::NONE);

			static std::list<Format> FindAllTilingOptimal(Context& context, const Properties& requiredProperties, const Features& requiredFeatures, const Properties& except = Properties::NONE);
			static std::list<Format> FindAllTilingLinear(Context& context, const Properties& requiredProperties, const Features& requiredFeatures, const Properties& except = Properties::NONE);
			static std::list<Format> FindAllBuffer(Context& context, const Properties& requiredProperties, const Features& requiredFeatures, const Properties& except = Properties::NONE);


		private:
			Type _type;
	};
}

#endif