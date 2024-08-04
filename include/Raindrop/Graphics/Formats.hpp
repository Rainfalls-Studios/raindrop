#ifndef __RAINDROP_GRAPHICS_FORMATS_HPP__
#define __RAINDROP_GRAPHICS_FORMATS_HPP__

#include "pch.pch"
#include "types.hpp"

namespace Raindrop::Graphics{
	class Formats{
		public:
			enum Properties : std::uint64_t{
				NONE = 0ULL,
				SFLOAT = 1ULL << 0ULL,
				SINT = 1ULL << 1ULL,
				SNORM = 1ULL << 2ULL,
				SSCALED = 1ULL << 3ULL,
				UFLOAT = 1ULL << 4ULL,
				UINT = 1ULL << 5ULL,
				UNORM = 1ULL << 6ULL,
				USCALED = 1ULL << 7ULL,
				SAMPLED_INT = 1ULL << 8ULL,
				SAMPLED_FLOAT = 1ULL << 9ULL,
				COMPRESSED_ASTC_HDR = 1ULL << 10ULL,
				COMPRESSED_ASTC_LDR = 1ULL << 11ULL,
				COMPRESSED_BC = 1ULL << 12ULL,
				COMPRESSED_EAC = 1ULL << 13ULL,
				COMPRESSED_ETC2 = 1ULL << 14ULL,
				COMPRESSED_PVRTC = 1ULL << 15ULL,
				COMPRESSED = 1ULL << 16ULL,
				DEPTH = 1ULL << 17ULL,
				STENCIL = 1ULL << 18ULL,
				PACKED = 1ULL << 19ULL,
				X_CHORMA_SUBSAMPLED = 1ULL << 20ULL,
				Y_CHORMA_SUBSAMPLED = 1ULL << 21ULL,
				SINGLE_PLANE_422 = 1ULL << 22ULL,
				MULTIPLANE = 1ULL << 23ULL,
				BIT8 = 1ULL << 24ULL,
				BIT16 = 1ULL << 25ULL,
				BIT32 = 1ULL << 26ULL,
				BIT64 = 1ULL << 27ULL,
				RED = 1ULL << 28ULL,
				GREEN = 1ULL << 29ULL,
				BLUE = 1ULL << 30ULL,
				ALPHA = 1ULL << 31ULL,
				REQUIRES_YBCBR_CONVERSION = 1ULL << 32ULL,

				NOT_COMPRESSED = 1ULL << 33ULL,
				NOT_MULTIPLANE = 1ULL << 34ULL,
				NOT_ALPHA = 1ULL << 35ULL,
				NOT_BLUE = 1ULL << 36ULL,
				NOT_GREEN = 1ULL << 37ULL,
				NOT_RED = 1ULL << 38ULL,
				NOT_DEPTH = 1ULL << 39ULL,
				NOT_STENCIL = 1ULL << 40ULL,
				NOT_PACKED = 1ULL << 41ULL,
				NOT_SINGLE_PLANE = 1ULL << 42ULL,

				R = RED,
				G = GREEN,
				B = BLUE,
				A = ALPHA,

				RG = R | G,
				RGB =  R | G | B,
				RGBA = R | G | B | A
			};
			using PropertiesFlags = std::uint64_t;

			Formats() noexcept;
			~Formats();

			Formats& prepare(Context& context);
			void initialize();
			void release();

			static bool isSFloat(const VkFormat& format) noexcept;
			static bool isSInt(const VkFormat& format) noexcept;
			static bool isSNorm(const VkFormat& format) noexcept;
			static bool isSScaled(const VkFormat& format) noexcept;
			static bool isUFloat(const VkFormat& format) noexcept;
			static bool isUInt(const VkFormat& format) noexcept;
			static bool isUNorm(const VkFormat& format) noexcept;
			static bool isUScaled(const VkFormat& format) noexcept;
			static bool isSampledInt(const VkFormat& format) noexcept;
			static bool isSampledFloat(const VkFormat& format) noexcept;
			static bool isCompressed_ASTC_HDR(const VkFormat& format) noexcept;
			static bool isCompressed_ASTC_LDR(const VkFormat& format) noexcept;
			static bool isCompressed_BC(const VkFormat& format) noexcept;
			static bool isCompressed_EAC(const VkFormat& format) noexcept;
			static bool isCompressed_ETC2(const VkFormat& format) noexcept;
			static bool isCompressed_PVRTC(const VkFormat& format) noexcept;
			static bool isCompressed(const VkFormat& format) noexcept;
			static bool isPacked(const VkFormat& format) noexcept;
			static bool isXChromaSubsampled(const VkFormat& format) noexcept;
			static bool isYChromaSubsampled(const VkFormat& format) noexcept;
			static bool isSinglePlane_422(const VkFormat& format) noexcept;
			static bool isMultiplane(const VkFormat& format) noexcept;
			static bool is8Bit(const VkFormat& format) noexcept;
			static bool is16Bit(const VkFormat& format) noexcept;
			static bool is32Bit(const VkFormat& format) noexcept;
			static bool is64Bit(const VkFormat& format) noexcept;
			static bool isColor(const VkFormat& format) noexcept;
			static bool isDepthOnly(const VkFormat& format) noexcept;
			static bool isStencilOnly(const VkFormat& format) noexcept;
			static bool requiresYbcbrConversion(const VkFormat& format) noexcept;
			static bool isNotCompressed(const VkFormat& format) noexcept;
			static bool isNotMultiplane(const VkFormat& format) noexcept;
			static bool isNotPacked(const VkFormat& format) noexcept;
			static bool isNotSinglePlane(const VkFormat& format) noexcept;

			static bool hasDepth(const VkFormat& format) noexcept;
			static bool hasStencil(const VkFormat& format) noexcept;
			static bool hasRed(const VkFormat& format) noexcept;
			static bool hasGreen(const VkFormat& format) noexcept;
			static bool hasBlue(const VkFormat& format) noexcept;
			static bool hasAlpha(const VkFormat& format) noexcept;

			static bool hasNotAlpha(const VkFormat& format) noexcept;
			static bool hasNotBlue(const VkFormat& format) noexcept;
			static bool hasNotGreen(const VkFormat& format) noexcept;
			static bool hasNotRed(const VkFormat& format) noexcept;
			static bool hasNotDepth(const VkFormat& format) noexcept;
			static bool hasNotStencil(const VkFormat& format) noexcept;


			static std::size_t getComponentCount(const VkFormat& format) noexcept;
			static std::size_t getPlaneCount(const VkFormat& format) noexcept;
			static std::size_t getDepthSize(const VkFormat& format) noexcept;
			static std::size_t getStencilSize(const VkFormat& format) noexcept;
			static std::size_t getSize(const VkFormat& format) noexcept;
			static std::size_t getTexelCount(const VkFormat& format) noexcept;
			static PropertiesFlags getProperties(const VkFormat& format) noexcept;

			enum Usage{
				BUFFER,
				LINEAR,
				OPTIMAL
			};

			bool isFormatSupported(const VkFormat& format, const PropertiesFlags& properties, const VkFormatFeatureFlags& features, const Usage& usage);
			VkFormat findFirstSuitableFormat(const PropertiesFlags& properties, const VkFormatFeatureFlags& features, const Usage& usage);
			VkFormat findFirstSuitableOptimalFormat(const PropertiesFlags& properties, const VkFormatFeatureFlags& features);
			VkFormat findFirstSuitableLinearFormat(const PropertiesFlags& properties, const VkFormatFeatureFlags& features);
			VkFormat findFirstSuitableBufferFormat(const PropertiesFlags& properties, const VkFormatFeatureFlags& features);

			VkFormat findBestFormat(const PropertiesFlags& properties, const VkFormatFeatureFlags& features, const Usage& usage);
			VkFormat findBestOptimalFormat(const PropertiesFlags& properties, const VkFormatFeatureFlags& features);
			VkFormat findBestLinearFormat(const PropertiesFlags& properties, const VkFormatFeatureFlags& features);
			VkFormat findBestBufferFormat(const PropertiesFlags& properties, const VkFormatFeatureFlags& features);

		
		private:
			Context* _context;
	};
}

#endif