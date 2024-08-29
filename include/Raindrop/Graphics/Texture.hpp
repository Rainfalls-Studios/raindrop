#ifndef __RAINDROP_GRAPHICS_TEXTURE_HPP__
#define __RAINDROP_GRAPHICS_TEXTURE_HPP__

#include "types.hpp"
#include "pch.pch"
#include <Raindrop/Assets/Asset.hpp>

namespace Raindrop::Graphics{
	class Texture : public Assets::Asset{
		public:
			static std::shared_ptr<Texture> create(Raindrop::Context& context);

			Texture() noexcept;
			~Texture() = default;

			void prepare(Context& context);
			void allocate();
			void release();

			void setData(const void* data);

			const std::shared_ptr<Image>& getImage() const noexcept;
			const std::shared_ptr<ImageView>& getImageView() const noexcept;
			const std::shared_ptr<Sampler>& getSampler() const noexcept;

			Texture& setWidth(const std::uint32_t& width);
			Texture& setHeight(const std::uint32_t& height);
			Texture& setDepth(const std::uint32_t& depth);
			Texture& setLayers(const std::uint32_t& layers);
			Texture& setBaseLayer(const std::uint32_t& baseLayer);
			Texture& setFormat(const VkFormat& format);
			Texture& setMipLayers(const std::uint32_t& mipLayers);
			Texture& setBaseMip(const std::uint32_t& baseMip);
			Texture& setSampleCount(const VkSampleCountFlagBits& sampleCount);
			Texture& setUsage(const VkImageUsageFlagBits& usage);
			Texture& setMagnificationFilter(const VkFilter& magnificationFilter);
			Texture& setMinificationFilter(const VkFilter& minificationFilter);
			Texture& setAddressModeU(const VkSamplerAddressMode& addressModeU);
			Texture& setAddressModeV(const VkSamplerAddressMode& addressModeV);
			Texture& setAddressModeW(const VkSamplerAddressMode& addressModeW);
			Texture& setMipmapSampleMode(const VkSamplerMipmapMode& mipmapSampleMode);
			Texture& setMaxAnisotropy(const float& maxAnisotropy);
			Texture& setAnisotropyEnable(const bool& anisotropyEnable);
			Texture& setBorderColor(const VkBorderColor& borderColor);
			Texture& setMinLod(const float& minLod);
			Texture& setMaxLod(const float& maxLod);
			Texture& setLodBias(const float& lodBias);
			Texture& setCompareOperation(const VkCompareOp& compareOperation);
			Texture& setCompareEnable(const bool& compareEnable);
			Texture& setUnormalizedCoords(const bool& unormalizedCoords);
			Texture& setTiling(const VkImageTiling& tiling);
			Texture& setSharingMode(const VkSharingMode& sharingMode);
			Texture& setFamilies(const std::vector<uint32_t>& families);
			Texture& setLayout(const VkImageLayout& layout);
			Texture& setMemoryProperties(const VkMemoryPropertyFlags& memoryProperties);
			Texture& setMemoryOffset(const std::uint32_t& memoryOffset);
			Texture& setViewType(const VkImageViewType& viewType);
			Texture& setImageType(const VkImageType& imageType);
			Texture& setAspectMask(const VkImageAspectFlags& aspectMask);
			Texture& setComponentMapping(const VkComponentMapping& componentMapping);
			Texture& setMemory(const std::shared_ptr<Memory>& memory);
			Texture& setImage(const std::shared_ptr<Image>& image);
			Texture& setImageView(const std::shared_ptr<ImageView>& imageView);
			Texture& setSampler(const std::shared_ptr<Sampler>& sampler);

		protected:
			struct BuildInfo{
				std::uint32_t width;
				std::uint32_t height;
				std::uint32_t depth;
				std::uint32_t layers;
				std::uint32_t baseLayer;
				VkFormat format;
				std::uint32_t mipLayers;
				std::uint32_t baseMip;
				VkSampleCountFlagBits sampleCount;
				VkImageUsageFlagBits usage;
				VkFilter magnificationFilter;
				VkFilter minificationFilter;
				VkSamplerAddressMode addressModeU;
				VkSamplerAddressMode addressModeV;
				VkSamplerAddressMode addressModeW;
				VkSamplerMipmapMode mipmapSampleMode;
				float maxAnisotropy;
				bool anisotropyEnable;
				VkBorderColor borderColor;
				float minLod;
				float maxLod;
				float lodBias;
				VkCompareOp compareOperation;
				bool compareEnable;
				bool unormalizedCoords;
				VkImageTiling tiling;
				VkSharingMode sharingMode;
				std::vector<uint32_t> families;
				VkImageLayout layout;
				VkMemoryPropertyFlags memoryProperties;
				std::uint32_t memoryOffset;
				VkImageViewType viewType;
				VkImageType imageType;
				VkImageAspectFlags aspectMask;
				VkComponentMapping componentMapping;

				std::shared_ptr<Memory> memory;
				std::shared_ptr<Image> image;
				std::shared_ptr<ImageView> imageView;
				std::shared_ptr<Sampler> sampler;

				BuildInfo() noexcept;
			} _info; 

			Context* _context;

			std::shared_ptr<Image> _image;
			std::shared_ptr<ImageView> _imageView;
			std::shared_ptr<Sampler> _sampler;

			std::unique_ptr<BuildInfo> _buildInfo;
			BuildInfo& getInfo();

			void createImage(const BuildInfo& info);
			void createImageView(const BuildInfo& info);
			void createSampler(const BuildInfo& info);
	};

	using Texture1D = Texture;
	using Texture2D = Texture;
	using Texture3D = Texture;
	using TextureCubemap = Texture;

	using Texture1DArray = Texture;
	using Texture2DArray = Texture;
	using TextureCubemapArray = Texture;
}

#endif