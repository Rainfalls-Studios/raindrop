#include <Raindrop/Graphics/Texture.hpp>
#include <Raindrop/Graphics/Context.hpp>
#include <Raindrop/Graphics/Image.hpp>
#include <Raindrop/Graphics/ImageView.hpp>
#include <Raindrop/Graphics/Sampler.hpp>
#include <Raindrop/Graphics/Buffer.hpp>
#include <Raindrop/Graphics/CommandBuffer.hpp>
#include <Raindrop/Exceptions/VulkanExceptions.hpp>
#include <Raindrop/Context.hpp>

namespace Raindrop::Graphics{
	std::shared_ptr<Texture> Texture::create(Raindrop::Context& context){
		return context.registry.emplace<Texture>();
	}

	Texture::BuildInfo::BuildInfo() noexcept : 
		width{1},
		height{1},
		depth{1},
		layers{1},
		baseLayer{0},
		format{VK_FORMAT_UNDEFINED},
		mipLayers{1},
		baseMip{0},
		sampleCount{VK_SAMPLE_COUNT_1_BIT},
		usage{VK_IMAGE_USAGE_SAMPLED_BIT},
		magnificationFilter{VK_FILTER_LINEAR},
		minificationFilter{VK_FILTER_LINEAR},
		addressModeU{VK_SAMPLER_ADDRESS_MODE_REPEAT},
		addressModeV{VK_SAMPLER_ADDRESS_MODE_REPEAT},
		addressModeW{VK_SAMPLER_ADDRESS_MODE_REPEAT},
		mipmapSampleMode{VK_SAMPLER_MIPMAP_MODE_LINEAR},
		maxAnisotropy{1.f},
		anisotropyEnable{false},
		borderColor{VK_BORDER_COLOR_INT_OPAQUE_BLACK},
		minLod{0.f},
		maxLod{1.f},
		lodBias{1.f},
		compareOperation{VK_COMPARE_OP_NEVER},
		compareEnable{false},
		unormalizedCoords{false},
		tiling{VK_IMAGE_TILING_LINEAR},
		sharingMode{VK_SHARING_MODE_EXCLUSIVE},
		families{},
		layout{VK_IMAGE_LAYOUT_UNDEFINED},
		memoryProperties{VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT},
		memoryOffset{0},
		viewType{VK_IMAGE_VIEW_TYPE_2D},
		imageType{VK_IMAGE_TYPE_2D},
		aspectMask{VK_IMAGE_ASPECT_COLOR_BIT},
		componentMapping{
			.r = VK_COMPONENT_SWIZZLE_R,
			.g = VK_COMPONENT_SWIZZLE_G,
			.b = VK_COMPONENT_SWIZZLE_B,
			.a = VK_COMPONENT_SWIZZLE_A
		},

		memory{},
		image{},
		imageView{},
		sampler{}
	{}

	Texture::Texture() noexcept : 
		_context{nullptr},
		_image{},
		_imageView{},
		_sampler{},
		_buildInfo{}
	{}

	const std::shared_ptr<Image>& Texture::getImage() const noexcept{
		return _image;	
	}

	const std::shared_ptr<ImageView>& Texture::getImageView() const noexcept{
		return _imageView;	
	}

	const std::shared_ptr<Sampler>& Texture::getSampler() const noexcept{
		return _sampler;	
	}

	Texture::BuildInfo& Texture::getInfo(){
		if (!_context || !_buildInfo){
			(_context != nullptr ? _context->logger : spdlog::default_logger())->warn("Attempt to access build info of a non prepared texture");
			throw std::runtime_error("The texture as not been prepared");
		}
		return *_buildInfo;
	}

	Texture& Texture::setWidth(const std::uint32_t& width){
		getInfo().width = width;
		return *this;
	}

	Texture& Texture::setHeight(const std::uint32_t& height){
		getInfo().height = height;
		return *this;
	}

	Texture& Texture::setDepth(const std::uint32_t& depth){
		getInfo().depth = depth;
		return *this;
	}


	Texture& Texture::setLayers(const std::uint32_t& layers){
		getInfo().layers = layers;
		return *this;
	}

	Texture& Texture::setBaseLayer(const std::uint32_t& baseLayer){
		getInfo().baseLayer = baseLayer;
		return *this;
	}

	Texture& Texture::setFormat(const VkFormat& format){
		getInfo().format = format;
		return *this;
	}

	Texture& Texture::setMipLayers(const std::uint32_t& mipLayers){
		getInfo().mipLayers = mipLayers;
		return *this;
	}

	Texture& Texture::setBaseMip(const std::uint32_t& baseMip){
		getInfo().baseMip = baseMip;
		return *this;
	}

	Texture& Texture::setSampleCount(const VkSampleCountFlagBits& sampleCount){
		getInfo().sampleCount = sampleCount;
		return *this;
	}

	Texture& Texture::setUsage(const VkImageUsageFlagBits& usage){
		getInfo().usage = usage;
		return *this;
	}

	Texture& Texture::setMagnificationFilter(const VkFilter& magnificationFilter){
		getInfo().magnificationFilter = magnificationFilter;
		return *this;
	}

	Texture& Texture::setMinificationFilter(const VkFilter& minificationFilter){
		getInfo().minificationFilter = minificationFilter;
		return *this;
	}

	Texture& Texture::setAddressModeU(const VkSamplerAddressMode& addressModeU){
		getInfo().addressModeU = addressModeU;
		return *this;
	}

	Texture& Texture::setAddressModeV(const VkSamplerAddressMode& addressModeV){
		getInfo().addressModeV = addressModeV;
		return *this;
	}

	Texture& Texture::setAddressModeW(const VkSamplerAddressMode& addressModeW){
		getInfo().addressModeW = addressModeW;
		return *this;
	}

	Texture& Texture::setMipmapSampleMode(const VkSamplerMipmapMode& mipmapSampleMode){
		getInfo().mipmapSampleMode = mipmapSampleMode;
		return *this;
	}

	Texture& Texture::setMaxAnisotropy(const float& maxAnisotropy){
		getInfo().maxAnisotropy = maxAnisotropy;
		return *this;
	}

	Texture& Texture::setAnisotropyEnable(const bool& anisotropyEnable){
		getInfo().anisotropyEnable = anisotropyEnable;
		return *this;
	}

	Texture& Texture::setBorderColor(const VkBorderColor& borderColor){
		getInfo().borderColor = borderColor;
		return *this;
	}

	Texture& Texture::setMinLod(const float& minLod){
		getInfo().minLod = minLod;
		return *this;
	}

	Texture& Texture::setMaxLod(const float& maxLod){
		getInfo().maxLod = maxLod;
		return *this;
	}

	Texture& Texture::setLodBias(const float& lodBias){
		getInfo().lodBias = lodBias;
		return *this;
	}

	Texture& Texture::setCompareOperation(const VkCompareOp& compareOperation){
		getInfo().compareOperation = compareOperation;
		return *this;
	}

	Texture& Texture::setCompareEnable(const bool& compareEnable){
		getInfo().compareEnable = compareEnable;
		return *this;
	}

	Texture& Texture::setUnormalizedCoords(const bool& unormalizedCoords){
		getInfo().unormalizedCoords = unormalizedCoords;
		return *this;
	}

	Texture& Texture::setTiling(const VkImageTiling& tiling){
		getInfo().tiling = tiling;
		return *this;
	}

	Texture& Texture::setSharingMode(const VkSharingMode& sharingMode){
		getInfo().sharingMode = sharingMode;
		return *this;
	}

	Texture& Texture::setFamilies(const std::vector<uint32_t>& families){
		getInfo().families = families;
		return *this;
	}

	Texture& Texture::setLayout(const VkImageLayout& layout){
		getInfo().layout = layout;
		return *this;
	}

	Texture& Texture::setMemoryProperties(const VkMemoryPropertyFlags& memoryProperties){
		getInfo().memoryProperties = memoryProperties;
		return *this;
	}

	Texture& Texture::setMemoryOffset(const std::uint32_t& memoryOffset){
		getInfo().memoryOffset = memoryOffset;
		return *this;
	}

	Texture& Texture::setViewType(const VkImageViewType& viewType){
		getInfo().viewType = viewType;
		return *this;
	}

	Texture& Texture::setImageType(const VkImageType& imageType){
		getInfo().imageType = imageType;
		return *this;
	}

	Texture& Texture::setAspectMask(const VkImageAspectFlags& aspectMask){
		getInfo().aspectMask = aspectMask;
		return *this;
	}

	Texture& Texture::setComponentMapping(const VkComponentMapping& componentMapping){
		getInfo().componentMapping = componentMapping;
		return *this;
	}

	Texture& Texture::setMemory(const std::shared_ptr<Memory>& memory){
		getInfo().memory = memory;
		return *this;
	}

	Texture& Texture::setImage(const std::shared_ptr<Image>& image){
		getInfo().image = image;
		return *this;
	}

	Texture& Texture::setImageView(const std::shared_ptr<ImageView>& imageView){
		getInfo().imageView = imageView;
		return *this;
	}

	Texture& Texture::setSampler(const std::shared_ptr<Sampler>& sampler){
		getInfo().sampler = sampler;
		return *this;
	}


	void Texture::prepare(Context& context){
		_context = &context;
		_buildInfo = std::make_unique<BuildInfo>();
	}

	void Texture::allocate(){
		BuildInfo& info = getInfo();

		if (info.image){
			_image = info.image;
		} else {
			createImage(info);
		}

		if (info.imageView){
			_imageView = info.imageView;
		} else {
			createImageView(info);
		}

		if (info.sampler){
			_sampler = info.sampler;
		} else {
			createSampler(info);
		}

		_info = *_buildInfo;
		_buildInfo.reset();
	}

	void Texture::createImage(const BuildInfo& info){
		_image = Image::create(*_context->raindrop);

		_image->prepare(*_context)
			.setType(info.imageType)
			.setFormat(info.format)
			.setWidth(info.width)
			.setHeight(info.height)
			.setDepth(info.depth)
			.setMipmapLevels(info.mipLayers)
			.setArrayLayers(info.layers)
			.setSampleCount(info.sampleCount)
			.setTiling(info.tiling)
			.setUsage(info.usage)
			.setSharingMode(info.sharingMode)
			.setQueueFamilies(info.families)
			.setMemoryProperties(info.memoryProperties)
			.setMemoryOffset(info.memoryOffset)
			.setMemory(info.memory)
			.initialize();
	}

	void Texture::createImageView(const BuildInfo& info){
		_imageView = ImageView::create(*_context->raindrop);

		_imageView->prepare(*_context)
			.setSource(_image)
			.setType(info.viewType)
			.setFormat(info.format)
			.setComponentMapping(info.componentMapping)
			.setAspectMask(info.aspectMask)
			.setBaseMipmapLevel(info.baseMip)
			.setMipmapLevelCount(info.mipLayers)
			.setBaseArrayLayer(info.baseLayer)
			.setLayerCount(info.layers)
			.initialize();
	}

	void Texture::createSampler(const BuildInfo& info){
		_sampler = Sampler::create(*_context->raindrop);

		_sampler->prepare(*_context)
			.setMagFilter(info.magnificationFilter)
			.setMinFilter(info.minificationFilter)
			.setMaxAnisotropy(info.mipmapSampleMode)
			.setAddressModeU(info.addressModeU)
			.setAddressModeV(info.addressModeV)
			.setAddressModeW(info.addressModeW)
			.setMipLodBias(info.lodBias)
			.enableAnistotropy(info.anisotropyEnable)
			.setMaxAnisotropy(info.maxAnisotropy)
			.enableComparation(info.compareEnable)
			.setCompareOperation(info.compareOperation)
			.setMinLod(info.minLod)
			.setMaxLod(info.maxLod)
			.setBorderColor(info.borderColor)
			.unormalizedCoordonates(info.unormalizedCoords)
			.initialize();
	}

	void Texture::release(){
		_image.reset();
		_imageView.reset();
		_sampler.reset();
	}

	void Texture::setData(const void* data){
		auto memoryRequirements = _image->getMemoryRequirements();
	}

}