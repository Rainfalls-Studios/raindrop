#include <Raindrop/Format.hpp>
#include <Raindrop/Context.hpp>
#include <Raindrop/Graphics/Context.hpp>
#include <vulkan/utility/vk_format_utils.h>

namespace Raindrop{
	VkFormat typeToVkFormat(const Format::Type& type){
		return static_cast<VkFormat>(type);
	}

	VkFormatFeatureFlags featuresToVkFeatures(const Format::Features& features){
		return static_cast<VkFormatFeatureFlags>(features.get());
	}

	Format::Format() noexcept : _type{UNDEFINED}{}
	Format::Format(Type type) noexcept : _type{type}{}
	Format::Format(const Format& other) noexcept : _type{other._type}{}

	Format& Format::operator=(const Format& other) noexcept{
		_type = other._type;
		return *this;
	}

	Format& Format::operator=(const Format::Type& type) noexcept{
		_type = type;
		return *this;
	}

	bool Format::operator==(const Format& other) const noexcept{
		return _type == other._type;
	}

	bool Format::operator==(const Format::Type& type) const noexcept{
		return _type == type;
	}

	bool Format::operator!=(const Format& other) const noexcept{
		return _type != other._type;
	}

	bool Format::operator!=(const Format::Type& type) const noexcept{
		return _type != type;
	}

	Format::Type Format::get() const noexcept{
		return _type;
	}

	bool Format::isSFloat() const noexcept{
		return vkuFormatIsSFLOAT(typeToVkFormat(_type));
	}

	bool Format::isSInt() const noexcept{
		return vkuFormatIsSINT(typeToVkFormat(_type));
	}

	bool Format::isSNorm() const noexcept{
		return vkuFormatIsSNORM(typeToVkFormat(_type));
	}

	bool Format::isSScaled() const noexcept{
		return vkuFormatIsSSCALED(typeToVkFormat(_type));
	}

	bool Format::isUFloat() const noexcept{
		return vkuFormatIsUFLOAT(typeToVkFormat(_type));
	}

	bool Format::isUInt() const noexcept{
		return vkuFormatIsUINT(typeToVkFormat(_type));
	}

	bool Format::isUNorm() const noexcept{
		return vkuFormatIsUNORM(typeToVkFormat(_type));
	}

	bool Format::isUScaled() const noexcept{
		return vkuFormatIsUSCALED(typeToVkFormat(_type));
	}

	bool Format::isSampledInt() const noexcept{
		return vkuFormatIsSampledInt(typeToVkFormat(_type));
	}

	bool Format::isSampledFloat() const noexcept{
		return vkuFormatIsSampledFloat(typeToVkFormat(_type));
	}

	bool Format::isCompressed_ASTC_HDR() const noexcept{
		return vkuFormatIsCompressed_ASTC_HDR(typeToVkFormat(_type));
	}

	bool Format::isCompressed_ASTC_LDR() const noexcept{
		return vkuFormatIsCompressed_ASTC_LDR(typeToVkFormat(_type));
	}

	bool Format::isCompressed_BC() const noexcept{
		return vkuFormatIsCompressed_BC(typeToVkFormat(_type));
	}

	bool Format::isCompressed_EAC() const noexcept{
		return vkuFormatIsCompressed_EAC(typeToVkFormat(_type));
	}

	bool Format::isCompressed_ETC2() const noexcept{
		return vkuFormatIsCompressed_ETC2(typeToVkFormat(_type));
	}

	bool Format::isCompressed_PVRTC() const noexcept{
		return vkuFormatIsCompressed_PVRTC(typeToVkFormat(_type));
	}

	bool Format::isCompressed() const noexcept{
		return vkuFormatIsCompressed(typeToVkFormat(_type));
	}

	bool Format::hasDepth() const noexcept{
		return vkuFormatHasDepth(typeToVkFormat(_type));
	}

	bool Format::hasStencil() const noexcept{
		return vkuFormatHasStencil(typeToVkFormat(_type));
	}

	bool Format::isPacked() const noexcept{
		return vkuFormatIsPacked(typeToVkFormat(_type));
	}

	bool Format::isXChromaSubsampled() const noexcept{
		return vkuFormatIsXChromaSubsampled(typeToVkFormat(_type));
	}

	bool Format::isYChromaSubsampled() const noexcept{
		return vkuFormatIsYChromaSubsampled(typeToVkFormat(_type));
	}

	bool Format::isSinglePlane_422() const noexcept{
		return vkuFormatIsSinglePlane_422(typeToVkFormat(_type));
	}

	bool Format::isMultiplane() const noexcept{
		return vkuFormatIsMultiplane(typeToVkFormat(_type));
	}

	bool Format::is8Bit() const noexcept{
		return vkuFormatIs8bit(typeToVkFormat(_type));
	}

	bool Format::is16Bit() const noexcept{
		return vkuFormatIs16bit(typeToVkFormat(_type));
	}

	bool Format::is32Bit() const noexcept{
		return vkuFormatIs32bit(typeToVkFormat(_type));
	}

	bool Format::is64Bit() const noexcept{
		return vkuFormatIs64bit(typeToVkFormat(_type));
	}

	bool Format::isColor() const noexcept{
		return vkuFormatIsColor(typeToVkFormat(_type));
	}

	bool Format::hasRed() const noexcept{
		return vkuFormatHasRed(typeToVkFormat(_type));
	}

	bool Format::hasGreen() const noexcept{
		return vkuFormatHasGreen(typeToVkFormat(_type));
	}

	bool Format::hasBlue() const noexcept{
		return vkuFormatHasBlue(typeToVkFormat(_type));
	}

	bool Format::hasAlpha() const noexcept{
		return vkuFormatHasAlpha(typeToVkFormat(_type));
	}

	bool Format::isDepthOnly() const noexcept{
		return vkuFormatIsDepthOnly(typeToVkFormat(_type));
	}

	bool Format::isStencilOnly() const noexcept{
		return vkuFormatIsStencilOnly(typeToVkFormat(_type));
	}

	bool Format::requiresYbcbrConversion() const noexcept{
		return vkuFormatRequiresYcbcrConversion(typeToVkFormat(_type));
	}

	std::size_t Format::getComponentCount() const noexcept{
		return static_cast<std::size_t>(vkuFormatComponentCount(typeToVkFormat(_type)));
	}

	std::size_t Format::getPlaneCount() const noexcept{
		return static_cast<std::size_t>(vkuFormatPlaneCount(typeToVkFormat(_type)));
	}

	std::size_t Format::getDepthSize() const noexcept{
		return static_cast<std::size_t>(vkuFormatDepthSize(typeToVkFormat(_type)));
	}

	std::size_t Format::getStencilSize() const noexcept{
		return static_cast<std::size_t>(vkuFormatStencilSize(typeToVkFormat(_type)));
	}

	std::size_t Format::getSize() const noexcept{
		return static_cast<std::size_t>(vkuGetFormatInfo(typeToVkFormat(_type)).block_size);
	}

	std::size_t Format::getTexelCount() const noexcept{
		return static_cast<std::size_t>(vkuGetFormatInfo(typeToVkFormat(_type)).texel_per_block);
	}

	Format::Properties Format::getProperties() const noexcept{
		Properties props;
		
		if (isSFloat()) 				props |= Properties::SFLOAT;
		if (isSInt()) 					props |= Properties::SINT;
		if (isSNorm()) 					props |= Properties::SNORM;
		if (isSScaled()) 				props |= Properties::SSCALED;
		if (isUFloat()) 				props |= Properties::UFLOAT;
		if (isUInt()) 					props |= Properties::UINT;
		if (isUNorm()) 					props |= Properties::UNORM;
		if (isUScaled()) 				props |= Properties::USCALED;
		if (isSampledInt()) 			props |= Properties::SAMPLED_INT;
		if (isSampledFloat()) 			props |= Properties::SAMPLED_FLOAT;
		if (isCompressed_ASTC_HDR())	props |= Properties::COMPRESSED_ASTC_HDR;
		if (isCompressed_ASTC_LDR())	props |= Properties::COMPRESSED_ASTC_LDR;
		if (isCompressed_BC()) 			props |= Properties::COMPRESSED_BC;
		if (isCompressed_EAC()) 		props |= Properties::COMPRESSED_EAC;
		if (isCompressed_ETC2()) 		props |= Properties::COMPRESSED_ETC2;
		if (isCompressed_PVRTC()) 		props |= Properties::COMPRESSED_PVRTC;
		if (isCompressed()) 			props |= Properties::COMPRESSED;
		if (hasDepth()) 				props |= Properties::DEPTH;
		if (hasStencil()) 				props |= Properties::STENCIL;
		if (isPacked()) 				props |= Properties::PACKED;
		if (isXChromaSubsampled()) 		props |= Properties::X_CHORMA_SUBSAMPLED;
		if (isYChromaSubsampled()) 		props |= Properties::Y_CHORMA_SUBSAMPLED;
		if (isSinglePlane_422()) 		props |= Properties::SINGLE_PLANE_422;
		if (isMultiplane()) 			props |= Properties::MULTIPLANE;
		if (is8Bit()) 					props |= Properties::BIT8;
		if (is16Bit()) 					props |= Properties::BIT16;
		if (is32Bit()) 					props |= Properties::BIT32;
		if (is64Bit()) 					props |= Properties::BIT64;
		if (hasRed()) 					props |= Properties::RED;
		if (hasGreen()) 				props |= Properties::GREEN;
		if (hasBlue()) 					props |= Properties::BLUE;
		if (hasAlpha()) 				props |= Properties::ALPHA;
		if (requiresYbcbrConversion()) 	props |= Properties::REQUIRES_YBCBR_CONVERSION;
		return props;
	}
	
	enum FormatUsage{
		BUFFER,
		LINEAR,
		OPTIMAL
	};

	bool isFormatSupported(Context& context, const Format& format, const Format::Properties& required, const Format::Features& features, const Format::Properties& except, const FormatUsage& usage){
		const auto& physicalDevice = context.renderer.context().physicalDevice;
		const auto& properties = format.getProperties();

		// If the format does not support ALL required properties, we skip this format
		if ((properties & required) != required) return false;

		// If the format supports at least one non wanted property, we also skip the format
		if (properties & except) return false;

		VkFormatProperties formatProps;
		vkGetPhysicalDeviceFormatProperties(physicalDevice.get(), typeToVkFormat(format.get()), &formatProps);

		VkFormatFeatureFlags supportedFeatures = 0;
		switch (usage){
			case BUFFER: supportedFeatures = formatProps.bufferFeatures; break;
			case LINEAR: supportedFeatures = formatProps.linearTilingFeatures; break;
			case OPTIMAL: supportedFeatures = formatProps.optimalTilingFeatures; break;
		}

		// Same as required propeties, we check that ALL required features are supported
		if ((featuresToVkFeatures(features) & supportedFeatures) != supportedFeatures) return false;

		return true;
	}

	Format findFormat(Context& context, const Format::Properties& required, const Format::Features& features, const Format::Properties& except, const FormatUsage& usage){
		for (std::size_t i=0; i<static_cast<std::size_t>(Format::ASTC_12x12_SRGB_BLOCK); i++){
			Format format(static_cast<Format::Type>(i));
			if (isFormatSupported(context, format ,required, features, except, usage)) return format;
		}
		return Format::UNDEFINED;
	}

	Format Format::findTilingOptimal(Context& context, const Properties& requiredProperties, const Features& requiredFeatures, const Properties& except){
		return findFormat(context, requiredProperties, requiredFeatures, except, OPTIMAL);
	}

	Format Format::findTilingLinear(Context& context, const Properties& requiredProperties, const Features& requiredFeatures, const Properties& except){
		return findFormat(context, requiredProperties, requiredFeatures, except, LINEAR);
	}

	Format Format::findBuffer(Context& context, const Properties& requiredProperties, const Features& requiredFeatures, const Properties& except){
		return findFormat(context, requiredProperties, requiredFeatures, except, BUFFER);
	}
}