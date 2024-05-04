#ifndef __RAINDROP_FORMAT_HPP__
#define __RAINDROP_FORMAT_HPP__

#include "common.hpp"
#include "Utils/Flags.hpp"

namespace Raindrop{
	class Format{
		public:
			// Based out of VkFormat
			enum Type{
				UNDEFINED = 0,
				R4G4_UNORM_PACK8 = 1,
				R4G4B4A4_UNORM_PACK16 = 2,
				B4G4R4A4_UNORM_PACK16 = 3,
				R5G6B5_UNORM_PACK16 = 4,
				B5G6R5_UNORM_PACK16 = 5,
				R5G5B5A1_UNORM_PACK16 = 6,
				B5G5R5A1_UNORM_PACK16 = 7,
				A1R5G5B5_UNORM_PACK16 = 8,
				R8_UNORM = 9,
				R8_SNORM = 10,
				R8_USCALED = 11,
				R8_SSCALED = 12,
				R8_UINT = 13,
				R8_SINT = 14,
				R8_SRGB = 15,
				R8G8_UNORM = 16,
				R8G8_SNORM = 17,
				R8G8_USCALED = 18,
				R8G8_SSCALED = 19,
				R8G8_UINT = 20,
				R8G8_SINT = 21,
				R8G8_SRGB = 22,
				R8G8B8_UNORM = 23,
				R8G8B8_SNORM = 24,
				R8G8B8_USCALED = 25,
				R8G8B8_SSCALED = 26,
				R8G8B8_UINT = 27,
				R8G8B8_SINT = 28,
				R8G8B8_SRGB = 29,
				B8G8R8_UNORM = 30,
				B8G8R8_SNORM = 31,
				B8G8R8_USCALED = 32,
				B8G8R8_SSCALED = 33,
				B8G8R8_UINT = 34,
				B8G8R8_SINT = 35,
				B8G8R8_SRGB = 36,
				R8G8B8A8_UNORM = 37,
				R8G8B8A8_SNORM = 38,
				R8G8B8A8_USCALED = 39,
				R8G8B8A8_SSCALED = 40,
				R8G8B8A8_UINT = 41,
				R8G8B8A8_SINT = 42,
				R8G8B8A8_SRGB = 43,
				B8G8R8A8_UNORM = 44,
				B8G8R8A8_SNORM = 45,
				B8G8R8A8_USCALED = 46,
				B8G8R8A8_SSCALED = 47,
				B8G8R8A8_UINT = 48,
				B8G8R8A8_SINT = 49,
				B8G8R8A8_SRGB = 50,
				A8B8G8R8_UNORM_PACK32 = 51,
				A8B8G8R8_SNORM_PACK32 = 52,
				A8B8G8R8_USCALED_PACK32 = 53,
				A8B8G8R8_SSCALED_PACK32 = 54,
				A8B8G8R8_UINT_PACK32 = 55,
				A8B8G8R8_SINT_PACK32 = 56,
				A8B8G8R8_SRGB_PACK32 = 57,
				A2R10G10B10_UNORM_PACK32 = 58,
				A2R10G10B10_SNORM_PACK32 = 59,
				A2R10G10B10_USCALED_PACK32 = 60,
				A2R10G10B10_SSCALED_PACK32 = 61,
				A2R10G10B10_UINT_PACK32 = 62,
				A2R10G10B10_SINT_PACK32 = 63,
				A2B10G10R10_UNORM_PACK32 = 64,
				A2B10G10R10_SNORM_PACK32 = 65,
				A2B10G10R10_USCALED_PACK32 = 66,
				A2B10G10R10_SSCALED_PACK32 = 67,
				A2B10G10R10_UINT_PACK32 = 68,
				A2B10G10R10_SINT_PACK32 = 69,
				R16_UNORM = 70,
				R16_SNORM = 71,
				R16_USCALED = 72,
				R16_SSCALED = 73,
				R16_UINT = 74,
				R16_SINT = 75,
				R16_SFLOAT = 76,
				R16G16_UNORM = 77,
				R16G16_SNORM = 78,
				R16G16_USCALED = 79,
				R16G16_SSCALED = 80,
				R16G16_UINT = 81,
				R16G16_SINT = 82,
				R16G16_SFLOAT = 83,
				R16G16B16_UNORM = 84,
				R16G16B16_SNORM = 85,
				R16G16B16_USCALED = 86,
				R16G16B16_SSCALED = 87,
				R16G16B16_UINT = 88,
				R16G16B16_SINT = 89,
				R16G16B16_SFLOAT = 90,
				R16G16B16A16_UNORM = 91,
				R16G16B16A16_SNORM = 92,
				R16G16B16A16_USCALED = 93,
				R16G16B16A16_SSCALED = 94,
				R16G16B16A16_UINT = 95,
				R16G16B16A16_SINT = 96,
				R16G16B16A16_SFLOAT = 97,
				R32_UINT = 98,
				R32_SINT = 99,
				R32_SFLOAT = 100,
				R32G32_UINT = 101,
				R32G32_SINT = 102,
				R32G32_SFLOAT = 103,
				R32G32B32_UINT = 104,
				R32G32B32_SINT = 105,
				R32G32B32_SFLOAT = 106,
				R32G32B32A32_UINT = 107,
				R32G32B32A32_SINT = 108,
				R32G32B32A32_SFLOAT = 109,
				R64_UINT = 110,
				R64_SINT = 111,
				R64_SFLOAT = 112,
				R64G64_UINT = 113,
				R64G64_SINT = 114,
				R64G64_SFLOAT = 115,
				R64G64B64_UINT = 116,
				R64G64B64_SINT = 117,
				R64G64B64_SFLOAT = 118,
				R64G64B64A64_UINT = 119,
				R64G64B64A64_SINT = 120,
				R64G64B64A64_SFLOAT = 121,
				B10G11R11_UFLOAT_PACK32 = 122,
				E5B9G9R9_UFLOAT_PACK32 = 123,
				D16_UNORM = 124,
				X8_D24_UNORM_PACK32 = 125,
				D32_SFLOAT = 126,
				S8_UINT = 127,
				D16_UNORM_S8_UINT = 128,
				D24_UNORM_S8_UINT = 129,
				D32_SFLOAT_S8_UINT = 130,
				BC1_RGB_UNORM_BLOCK = 131,
				BC1_RGB_SRGB_BLOCK = 132,
				BC1_RGBA_UNORM_BLOCK = 133,
				BC1_RGBA_SRGB_BLOCK = 134,
				BC2_UNORM_BLOCK = 135,
				BC2_SRGB_BLOCK = 136,
				BC3_UNORM_BLOCK = 137,
				BC3_SRGB_BLOCK = 138,
				BC4_UNORM_BLOCK = 139,
				BC4_SNORM_BLOCK = 140,
				BC5_UNORM_BLOCK = 141,
				BC5_SNORM_BLOCK = 142,
				BC6H_UFLOAT_BLOCK = 143,
				BC6H_SFLOAT_BLOCK = 144,
				BC7_UNORM_BLOCK = 145,
				BC7_SRGB_BLOCK = 146,
				ETC2_R8G8B8_UNORM_BLOCK = 147,
				ETC2_R8G8B8_SRGB_BLOCK = 148,
				ETC2_R8G8B8A1_UNORM_BLOCK = 149,
				ETC2_R8G8B8A1_SRGB_BLOCK = 150,
				ETC2_R8G8B8A8_UNORM_BLOCK = 151,
				ETC2_R8G8B8A8_SRGB_BLOCK = 152,
				EAC_R11_UNORM_BLOCK = 153,
				EAC_R11_SNORM_BLOCK = 154,
				EAC_R11G11_UNORM_BLOCK = 155,
				EAC_R11G11_SNORM_BLOCK = 156,
				ASTC_4x4_UNORM_BLOCK = 157,
				ASTC_4x4_SRGB_BLOCK = 158,
				ASTC_5x4_UNORM_BLOCK = 159,
				ASTC_5x4_SRGB_BLOCK = 160,
				ASTC_5x5_UNORM_BLOCK = 161,
				ASTC_5x5_SRGB_BLOCK = 162,
				ASTC_6x5_UNORM_BLOCK = 163,
				ASTC_6x5_SRGB_BLOCK = 164,
				ASTC_6x6_UNORM_BLOCK = 165,
				ASTC_6x6_SRGB_BLOCK = 166,
				ASTC_8x5_UNORM_BLOCK = 167,
				ASTC_8x5_SRGB_BLOCK = 168,
				ASTC_8x6_UNORM_BLOCK = 169,
				ASTC_8x6_SRGB_BLOCK = 170,
				ASTC_8x8_UNORM_BLOCK = 171,
				ASTC_8x8_SRGB_BLOCK = 172,
				ASTC_10x5_UNORM_BLOCK = 173,
				ASTC_10x5_SRGB_BLOCK = 174,
				ASTC_10x6_UNORM_BLOCK = 175,
				ASTC_10x6_SRGB_BLOCK = 176,
				ASTC_10x8_UNORM_BLOCK = 177,
				ASTC_10x8_SRGB_BLOCK = 178,
				ASTC_10x10_UNORM_BLOCK = 179,
				ASTC_10x10_SRGB_BLOCK = 180,
				ASTC_12x10_UNORM_BLOCK = 181,
				ASTC_12x10_SRGB_BLOCK = 182,
				ASTC_12x12_UNORM_BLOCK = 183,
				ASTC_12x12_SRGB_BLOCK = 184,
				// G8B8G8R8_422_UNORM = 1000156000,
				// B8G8R8G8_422_UNORM = 1000156001,
				// G8_B8_R8_3PLANE_420_UNORM = 1000156002,
				// G8_B8R8_2PLANE_420_UNORM = 1000156003,
				// G8_B8_R8_3PLANE_422_UNORM = 1000156004,
				// G8_B8R8_2PLANE_422_UNORM = 1000156005,
				// G8_B8_R8_3PLANE_444_UNORM = 1000156006,
				// R10X6_UNORM_PACK16 = 1000156007,
				// R10X6G10X6_UNORM_2PACK16 = 1000156008,
				// R10X6G10X6B10X6A10X6_UNORM_4PACK16 = 1000156009,
				// G10X6B10X6G10X6R10X6_422_UNORM_4PACK16 = 1000156010,
				// B10X6G10X6R10X6G10X6_422_UNORM_4PACK16 = 1000156011,
				// G10X6_B10X6_R10X6_3PLANE_420_UNORM_3PACK16 = 1000156012,
				// G10X6_B10X6R10X6_2PLANE_420_UNORM_3PACK16 = 1000156013,
				// G10X6_B10X6_R10X6_3PLANE_422_UNORM_3PACK16 = 1000156014,
				// G10X6_B10X6R10X6_2PLANE_422_UNORM_3PACK16 = 1000156015,
				// G10X6_B10X6_R10X6_3PLANE_444_UNORM_3PACK16 = 1000156016,
				// R12X4_UNORM_PACK16 = 1000156017,
				// R12X4G12X4_UNORM_2PACK16 = 1000156018,
				// R12X4G12X4B12X4A12X4_UNORM_4PACK16 = 1000156019,
				// G12X4B12X4G12X4R12X4_422_UNORM_4PACK16 = 1000156020,
				// B12X4G12X4R12X4G12X4_422_UNORM_4PACK16 = 1000156021,
				// G12X4_B12X4_R12X4_3PLANE_420_UNORM_3PACK16 = 1000156022,
				// G12X4_B12X4R12X4_2PLANE_420_UNORM_3PACK16 = 1000156023,
				// G12X4_B12X4_R12X4_3PLANE_422_UNORM_3PACK16 = 1000156024,
				// G12X4_B12X4R12X4_2PLANE_422_UNORM_3PACK16 = 1000156025,
				// G12X4_B12X4_R12X4_3PLANE_444_UNORM_3PACK16 = 1000156026,
				// G16B16G16R16_422_UNORM = 1000156027,
				// B16G16R16G16_422_UNORM = 1000156028,
				// G16_B16_R16_3PLANE_420_UNORM = 1000156029,
				// G16_B16R16_2PLANE_420_UNORM = 1000156030,
				// G16_B16_R16_3PLANE_422_UNORM = 1000156031,
				// G16_B16R16_2PLANE_422_UNORM = 1000156032,
				// G16_B16_R16_3PLANE_444_UNORM = 1000156033,
				// G8_B8R8_2PLANE_444_UNORM = 1000330000,
				// G10X6_B10X6R10X6_2PLANE_444_UNORM_3PACK16 = 1000330001,
				// G12X4_B12X4R12X4_2PLANE_444_UNORM_3PACK16 = 1000330002,
				// G16_B16R16_2PLANE_444_UNORM = 1000330003,
				// A4R4G4B4_UNORM_PACK16 = 1000340000,
				// A4B4G4R4_UNORM_PACK16 = 1000340001,
				// ASTC_4x4_SFLOAT_BLOCK = 1000066000,
				// ASTC_5x4_SFLOAT_BLOCK = 1000066001,
				// ASTC_5x5_SFLOAT_BLOCK = 1000066002,
				// ASTC_6x5_SFLOAT_BLOCK = 1000066003,
				// ASTC_6x6_SFLOAT_BLOCK = 1000066004,
				// ASTC_8x5_SFLOAT_BLOCK = 1000066005,
				// ASTC_8x6_SFLOAT_BLOCK = 1000066006,
				// ASTC_8x8_SFLOAT_BLOCK = 1000066007,
				// ASTC_10x5_SFLOAT_BLOCK = 1000066008,
				// ASTC_10x6_SFLOAT_BLOCK = 1000066009,
				// ASTC_10x8_SFLOAT_BLOCK = 1000066010,
				// ASTC_10x10_SFLOAT_BLOCK = 1000066011,
				// ASTC_12x10_SFLOAT_BLOCK = 1000066012,
				// ASTC_12x12_SFLOAT_BLOCK = 1000066013,
				// PVRTC1_2BPP_UNORM_BLOCK_IMG = 1000054000,
				// PVRTC1_4BPP_UNORM_BLOCK_IMG = 1000054001,
				// PVRTC2_2BPP_UNORM_BLOCK_IMG = 1000054002,
				// PVRTC2_4BPP_UNORM_BLOCK_IMG = 1000054003,
				// PVRTC1_2BPP_SRGB_BLOCK_IMG = 1000054004,
				// PVRTC1_4BPP_SRGB_BLOCK_IMG = 1000054005,
				// PVRTC2_2BPP_SRGB_BLOCK_IMG = 1000054006,
				// PVRTC2_4BPP_SRGB_BLOCK_IMG = 1000054007,
				// R16G16_S10_5_NV = 1000464000
			};
			
			class Properties : public Utils::FlagsTemplate<Properties, std::uint64_t> {
				public:
					using FlagsTemplate<Properties, std::uint64_t>::FlagsTemplate;

					enum Bits : Bitset {
						NONE = 0ULL,
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
			Format(Type type) noexcept;
			Format(const Format& other) noexcept;

			Format& operator=(const Format& other) noexcept;
			Format& operator=(const Format::Type& type) noexcept;

			bool operator==(const Format& other) const noexcept;
			bool operator==(const Format::Type& type) const noexcept;

			bool operator!=(const Format& other) const noexcept;
			bool operator!=(const Format::Type& type) const noexcept;

			Type get() const noexcept;

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

			static Format findTilingOptimal(Context& context, const Properties& requiredProperties, const Features& requiredFeatures, const Properties& except = Properties::NONE);
			static Format findTilingLinear(Context& context, const Properties& requiredProperties, const Features& requiredFeatures, const Properties& except = Properties::NONE);
			static Format findBuffer(Context& context, const Properties& requiredProperties, const Features& requiredFeatures, const Properties& except = Properties::NONE);

		private:
			Type _type;
	};
}

#endif