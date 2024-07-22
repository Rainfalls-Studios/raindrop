#include <Raindrop/Graphics/Formats.hpp>
#include <Raindrop/Graphics/Context.hpp>
#include "for_each_formats.cpp"

#include <vulkan/utility/vk_format_utils.h>


#include <vulkan/vk_enum_string_helper.h>
#include <iostream>
#include <bitset>

namespace Raindrop::Graphics{
	Formats::Formats() noexcept :
		_context{nullptr}
	{}

	Formats::~Formats(){
		release();
	}

	void Formats::prepare(Context& context){
		_context = &context;
	}

	void Formats::initialize(){

	}

	void Formats::release(){
		_context = nullptr;
	}

	bool Formats::isSFloat(const VkFormat& format) noexcept{
		return vkuFormatIsSFLOAT(format);
	}

	bool Formats::isSInt(const VkFormat& format) noexcept{
		return vkuFormatIsSINT(format);
	}

	bool Formats::isSNorm(const VkFormat& format) noexcept{
		return vkuFormatIsSNORM(format);
	}

	bool Formats::isSScaled(const VkFormat& format) noexcept{
		return vkuFormatIsSSCALED(format);
	}

	bool Formats::isUFloat(const VkFormat& format) noexcept{
		return vkuFormatIsUFLOAT(format);
	}

	bool Formats::isUInt(const VkFormat& format) noexcept{
		return vkuFormatIsUINT(format);
	}

	bool Formats::isUNorm(const VkFormat& format) noexcept{
		return vkuFormatIsUNORM(format);
	}

	bool Formats::isUScaled(const VkFormat& format) noexcept{
		return vkuFormatIsUSCALED(format);
	}

	bool Formats::isSampledInt(const VkFormat& format) noexcept{
		return vkuFormatIsSampledInt(format);
	}

	bool Formats::isSampledFloat(const VkFormat& format) noexcept{
		return vkuFormatIsSampledFloat(format);
	}

	bool Formats::isCompressed_ASTC_HDR(const VkFormat& format) noexcept{
		return vkuFormatIsCompressed_ASTC_HDR(format);
	}

	bool Formats::isCompressed_ASTC_LDR(const VkFormat& format) noexcept{
		return vkuFormatIsCompressed_ASTC_LDR(format);
	}

	bool Formats::isCompressed_BC(const VkFormat& format) noexcept{
		return vkuFormatIsCompressed_BC(format);
	}

	bool Formats::isCompressed_EAC(const VkFormat& format) noexcept{
		return vkuFormatIsCompressed_EAC(format);
	}

	bool Formats::isCompressed_ETC2(const VkFormat& format) noexcept{
		return vkuFormatIsCompressed_ETC2(format);
	}

	bool Formats::isCompressed_PVRTC(const VkFormat& format) noexcept{
		return vkuFormatIsCompressed_PVRTC(format);
	}

	bool Formats::isCompressed(const VkFormat& format) noexcept{
		return vkuFormatIsCompressed(format);
	}

	bool Formats::hasDepth(const VkFormat& format) noexcept{
		return vkuFormatHasDepth(format);
	}

	bool Formats::hasStencil(const VkFormat& format) noexcept{
		return vkuFormatHasStencil(format);
	}

	bool Formats::isPacked(const VkFormat& format) noexcept{
		return vkuFormatIsPacked(format);
	}

	bool Formats::isXChromaSubsampled(const VkFormat& format) noexcept{
		return vkuFormatIsXChromaSubsampled(format);
	}

	bool Formats::isYChromaSubsampled(const VkFormat& format) noexcept{
		return vkuFormatIsYChromaSubsampled(format);
	}

	bool Formats::isSinglePlane_422(const VkFormat& format) noexcept{
		return vkuFormatIsSinglePlane_422(format);
	}

	bool Formats::isMultiplane(const VkFormat& format) noexcept{
		return vkuFormatIsMultiplane(format);
	}

	bool Formats::is8Bit(const VkFormat& format) noexcept{
		return vkuFormatIs8bit(format);
	}

	bool Formats::is16Bit(const VkFormat& format) noexcept{
		return vkuFormatIs16bit(format);
	}

	bool Formats::is32Bit(const VkFormat& format) noexcept{
		return vkuFormatIs32bit(format);
	}

	bool Formats::is64Bit(const VkFormat& format) noexcept{
		return vkuFormatIs64bit(format);
	}

	bool Formats::isColor(const VkFormat& format) noexcept{
		return vkuFormatIsColor(format);
	}

	bool Formats::hasRed(const VkFormat& format) noexcept{
		return vkuFormatHasRed(format);
	}

	bool Formats::hasGreen(const VkFormat& format) noexcept{
		return vkuFormatHasGreen(format);
	}

	bool Formats::hasBlue(const VkFormat& format) noexcept{
		return vkuFormatHasBlue(format);
	}

	bool Formats::hasAlpha(const VkFormat& format) noexcept{
		return vkuFormatHasAlpha(format);
	}

	bool Formats::isDepthOnly(const VkFormat& format) noexcept{
		return vkuFormatIsDepthOnly(format);
	}

	bool Formats::isStencilOnly(const VkFormat& format) noexcept{
		return vkuFormatIsStencilOnly(format);
	}

	bool Formats::requiresYbcbrConversion(const VkFormat& format) noexcept{
		return vkuFormatRequiresYcbcrConversion(format);
	}

	bool Formats::isNotCompressed(const VkFormat& format) noexcept{
		return !isCompressed(format);
	}

	bool Formats::isNotMultiplane(const VkFormat& format) noexcept{
		return !isMultiplane(format);
	}

	bool Formats::isNotPacked(const VkFormat& format) noexcept{
		return !isPacked(format);
	}

	bool Formats::hasNotAlpha(const VkFormat& format) noexcept{
		return !hasAlpha(format);
	}

	bool Formats::hasNotBlue(const VkFormat& format) noexcept{
		return !hasBlue(format);
	}

	bool Formats::hasNotGreen(const VkFormat& format) noexcept{
		return !hasGreen(format);
	}

	bool Formats::hasNotRed(const VkFormat& format) noexcept{
		return !hasRed(format);
	}

	bool Formats::hasNotDepth(const VkFormat& format) noexcept{
		return !hasDepth(format);
	}

	bool Formats::hasNotStencil(const VkFormat& format) noexcept{
		return !hasStencil(format);
	}

	bool Formats::isNotSinglePlane(const VkFormat& format) noexcept{
		return !isSinglePlane_422(format);
	}

	std::size_t Formats::getComponentCount(const VkFormat& format) noexcept{
		return static_cast<std::size_t>(vkuFormatComponentCount(format));
	}

	std::size_t Formats::getPlaneCount(const VkFormat& format) noexcept{
		return static_cast<std::size_t>(vkuFormatPlaneCount(format));
	}

	std::size_t Formats::getDepthSize(const VkFormat& format) noexcept{
		return static_cast<std::size_t>(vkuFormatDepthSize(format));
	}

	std::size_t Formats::getStencilSize(const VkFormat& format) noexcept{
		return static_cast<std::size_t>(vkuFormatStencilSize(format));
	}

	std::size_t Formats::getSize(const VkFormat& format) noexcept{
		return static_cast<std::size_t>(vkuGetFormatInfo(format).block_size);
	}

	std::size_t Formats::getTexelCount(const VkFormat& format) noexcept{
		return static_cast<std::size_t>(vkuGetFormatInfo(format).texel_per_block);
	}

	Formats::PropertiesFlags Formats::getProperties(const VkFormat& format) noexcept{
		PropertiesFlags props = 0;
		
		if (isSFloat(format)) 					props |= static_cast<PropertiesFlags>(Properties::SFLOAT);
		if (isSInt(format)) 					props |= static_cast<PropertiesFlags>(Properties::SINT);
		if (isSNorm(format)) 					props |= static_cast<PropertiesFlags>(Properties::SNORM);
		if (isSScaled(format)) 					props |= static_cast<PropertiesFlags>(Properties::SSCALED);
		if (isUFloat(format)) 					props |= static_cast<PropertiesFlags>(Properties::UFLOAT);
		if (isUInt(format)) 					props |= static_cast<PropertiesFlags>(Properties::UINT);
		if (isUNorm(format)) 					props |= static_cast<PropertiesFlags>(Properties::UNORM);
		if (isUScaled(format)) 					props |= static_cast<PropertiesFlags>(Properties::USCALED);
		if (isSampledInt(format)) 				props |= static_cast<PropertiesFlags>(Properties::SAMPLED_INT);
		if (isSampledFloat(format)) 			props |= static_cast<PropertiesFlags>(Properties::SAMPLED_FLOAT);
		if (isCompressed_ASTC_HDR(format))		props |= static_cast<PropertiesFlags>(Properties::COMPRESSED_ASTC_HDR);
		if (isCompressed_ASTC_LDR(format))		props |= static_cast<PropertiesFlags>(Properties::COMPRESSED_ASTC_LDR);
		if (isCompressed_BC(format)) 			props |= static_cast<PropertiesFlags>(Properties::COMPRESSED_BC);
		if (isCompressed_EAC(format)) 			props |= static_cast<PropertiesFlags>(Properties::COMPRESSED_EAC);
		if (isCompressed_ETC2(format)) 			props |= static_cast<PropertiesFlags>(Properties::COMPRESSED_ETC2);
		if (isCompressed_PVRTC(format)) 		props |= static_cast<PropertiesFlags>(Properties::COMPRESSED_PVRTC);
		if (isCompressed(format)) 				props |= static_cast<PropertiesFlags>(Properties::COMPRESSED);
		if (hasDepth(format)) 					props |= static_cast<PropertiesFlags>(Properties::DEPTH);
		if (hasStencil(format)) 				props |= static_cast<PropertiesFlags>(Properties::STENCIL);
		if (isPacked(format)) 					props |= static_cast<PropertiesFlags>(Properties::PACKED);
		if (isXChromaSubsampled(format)) 		props |= static_cast<PropertiesFlags>(Properties::X_CHORMA_SUBSAMPLED);
		if (isYChromaSubsampled(format)) 		props |= static_cast<PropertiesFlags>(Properties::Y_CHORMA_SUBSAMPLED);
		if (isSinglePlane_422(format)) 			props |= static_cast<PropertiesFlags>(Properties::SINGLE_PLANE_422);
		if (isMultiplane(format)) 				props |= static_cast<PropertiesFlags>(Properties::MULTIPLANE);
		if (is8Bit(format)) 					props |= static_cast<PropertiesFlags>(Properties::BIT8);
		if (is16Bit(format)) 					props |= static_cast<PropertiesFlags>(Properties::BIT16);
		if (is32Bit(format)) 					props |= static_cast<PropertiesFlags>(Properties::BIT32);
		if (is64Bit(format)) 					props |= static_cast<PropertiesFlags>(Properties::BIT64);
		if (hasRed(format)) 					props |= static_cast<PropertiesFlags>(Properties::RED);
		if (hasGreen(format)) 					props |= static_cast<PropertiesFlags>(Properties::GREEN);
		if (hasBlue(format)) 					props |= static_cast<PropertiesFlags>(Properties::BLUE);
		if (hasAlpha(format)) 					props |= static_cast<PropertiesFlags>(Properties::ALPHA);
		if (requiresYbcbrConversion(format)) 	props |= static_cast<PropertiesFlags>(Properties::REQUIRES_YBCBR_CONVERSION);
		if (isNotCompressed(format))		 	props |= static_cast<PropertiesFlags>(Properties::NOT_COMPRESSED);
		if (isNotMultiplane(format)) 			props |= static_cast<PropertiesFlags>(Properties::NOT_MULTIPLANE);
		if (hasNotAlpha(format)) 				props |= static_cast<PropertiesFlags>(Properties::NOT_ALPHA);
		if (hasNotBlue(format)) 				props |= static_cast<PropertiesFlags>(Properties::NOT_BLUE);
		if (hasNotGreen(format)) 				props |= static_cast<PropertiesFlags>(Properties::NOT_GREEN);
		if (hasNotRed(format)) 					props |= static_cast<PropertiesFlags>(Properties::NOT_RED);
		if (hasNotDepth(format)) 				props |= static_cast<PropertiesFlags>(Properties::NOT_DEPTH);
		if (hasNotStencil(format)) 				props |= static_cast<PropertiesFlags>(Properties::NOT_STENCIL);
		if (isNotPacked(format)) 				props |= static_cast<PropertiesFlags>(Properties::NOT_PACKED);
		if (isNotSinglePlane(format)) 			props |= static_cast<PropertiesFlags>(Properties::NOT_SINGLE_PLANE);

		return props;
	}

	bool Formats::isFormatSupported(const VkFormat& format, const PropertiesFlags& requiredProperties, const VkFormatFeatureFlags& features, const Usage& usage){
		const auto& physicalDevice = _context->getPhysicalDevice();
		auto properties = getProperties(format);

		// If the Formats does not support ALL required properties, we skip this Formats
		if ((requiredProperties & properties) != requiredProperties) return false;

		VkFormatProperties formatProps;
		vkGetPhysicalDeviceFormatProperties(physicalDevice.get(), format, &formatProps);

		VkFormatFeatureFlags supportedFeatures = 0;
		switch (usage){
			case BUFFER: supportedFeatures = formatProps.bufferFeatures; break;
			case LINEAR: supportedFeatures = formatProps.linearTilingFeatures; break;
			case OPTIMAL: supportedFeatures = formatProps.optimalTilingFeatures; break;
		}

		// Same as required propeties, we check that ALL required features are supported
		if ((supportedFeatures & features) != features) return false;

		return true;
	}

	VkFormat Formats::findFirstSuitableFormat(const PropertiesFlags& properties, const VkFormatFeatureFlags& features, const Usage& usage){
		for_each_formats({
			if (isFormatSupported(fmt, properties, features, usage)) {return fmt;}
		});
		return VK_FORMAT_UNDEFINED;
	}

	VkFormat Formats::findFirstSuitableOptimalFormat(const PropertiesFlags& properties, const VkFormatFeatureFlags& features){
		return findFirstSuitableFormat(properties, features, OPTIMAL);
	}

	VkFormat Formats::findFirstSuitableLinearFormat(const PropertiesFlags& properties, const VkFormatFeatureFlags& features){
		return findFirstSuitableFormat(properties, features, LINEAR);
	}

	VkFormat Formats::findFirstSuitableBufferFormat(const PropertiesFlags& properties, const VkFormatFeatureFlags& features){
		return findFirstSuitableFormat(properties, features, BUFFER);
	}

	VkFormat Formats::findBestFormat(const PropertiesFlags& requiredProperties, const VkFormatFeatureFlags& features, const Usage& usage){
		const auto& physicalDevice = _context->getPhysicalDevice();

		// First step, load all formats
		std::list<std::pair<std::size_t, VkFormat>> formats;

		for_each_formats({
			auto properties = getProperties(fmt);

			if ((requiredProperties & properties) != requiredProperties) continue;

			VkFormatProperties formatProps;
			vkGetPhysicalDeviceFormatProperties(physicalDevice.get(), fmt, &formatProps);

			VkFormatFeatureFlags supportedFeatures = 0;
			switch (usage){
				case BUFFER: supportedFeatures = formatProps.bufferFeatures; break;
				case LINEAR: supportedFeatures = formatProps.linearTilingFeatures; break;
				case OPTIMAL: supportedFeatures = formatProps.optimalTilingFeatures; break;
			}

			// Same as required propeties, we check that ALL required features are supported
			if ((supportedFeatures & features) != features) continue;

			std::size_t score = 0;

			// clamp to avoid warp around and having a low score for a very 
			score += static_cast<std::size_t>(requiredProperties ^ properties);
			score += static_cast<std::size_t>(features ^ supportedFeatures);

			// _context->logger->info("{} : {}", string_VkFormat(fmt), score);

			formats.emplace_back(score, fmt);
		});

		auto it = std::min_element(formats.begin(), formats.end());
		if (it == formats.end()){
			return VK_FORMAT_UNDEFINED;
		}

		return it->second;
	}

	VkFormat Formats::findBestOptimalFormat(const PropertiesFlags& properties, const VkFormatFeatureFlags& features){
		return findBestFormat(properties, features, OPTIMAL);
	}

	VkFormat Formats::findBestLinearFormat(const PropertiesFlags& properties, const VkFormatFeatureFlags& features){
		return findBestFormat(properties, features, LINEAR);
	}

	VkFormat Formats::findBestBufferFormat(const PropertiesFlags& properties, const VkFormatFeatureFlags& features){
		return findBestFormat(properties, features, BUFFER);
	}
}