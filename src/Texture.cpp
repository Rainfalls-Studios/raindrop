#include <Raindrop/Texture.hpp>
#include <Raindrop_internal/Texture.hpp>
#include <Raindrop/GUID.hpp>
#include <Raindrop_internal/Graphics/Image.hpp>
#include <Raindrop_internal/Context.hpp>
#include <Raindrop_internal/Graphics/Renderer.hpp>
#include <Raindrop/Context.hpp>
#include <Raindrop/Exceptions/VulkanExceptions.hpp>
#include <Raindrop_internal/Format.hpp>
#include <vulkan/vk_enum_string_helper.h>

#define LOGGER _impl->context->getInternalContext()->getLogger()
#define GRAPHICS_CONTEXT _impl->context->getInternalContext()->getRenderer().getContext()

namespace Raindrop{
	//--------------------------------------------------------------------
	//-----------------           TEXTURE                -----------------
	//--------------------------------------------------------------------

	// Default configuration
	Texture::Impl::Builder::Builder() :
		info{
			.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
			.pNext = nullptr,
			.flags = 0,
			.imageType = VK_IMAGE_TYPE_2D,
			.format = VK_FORMAT_R8G8B8A8_UNORM,
			.extent{
				.width = 1024,
				.height = 1024,
				.depth = 1
			},
			.mipLevels = 1,
			.arrayLayers = 1,
			.samples = VK_SAMPLE_COUNT_1_BIT,
			.tiling = VK_IMAGE_TILING_OPTIMAL,
			.usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
			.sharingMode = VK_SHARING_MODE_EXCLUSIVE,
			.queueFamilyIndexCount = 0,
			.pQueueFamilyIndices = nullptr,
			.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
		}
	{}
	
	void Texture::Impl::transfertBuildDataToInfo(){
		info = Info{
			.format = FormatToRaindrop(builder->info.format),
			.width = builder->info.extent.width,
			.height = builder->info.extent.height,
			.depth = builder->info.extent.depth,
			.usage = TextureUsageToRaindrop(builder->info.usage),
			.layout = TextureLayoutToRaindrop(builder->info.initialLayout),
			.tiling = TextureTilingToRaindrop(builder->info.tiling),
			.type = TextureTypeToRaindrop(builder->info.imageType),
			.mipCount = builder->info.mipLevels,
			.arrLayers = builder->info.arrayLayers,
			.flags = TextureFlagsToRaindrop(builder->info.flags)
		};
	}

	static inline std::runtime_error access_error(){
		spdlog::error("Cannot access data of a non initialized texture");
		return std::runtime_error("The texture is not initialized");
	}

	const Texture::FormatProperties Texture::FormatProperties::UNSUPPORTED = Texture::FormatProperties{false, 0, 0, 0, 0, 0, 0};

	Texture Texture::Create(Context& context){
		return Texture(context);
	}

	Texture::Texture(Context& context) : _impl{std::make_unique<Impl>()}{
		_impl->builder = std::make_unique<Impl::Builder>();
		_impl->context = &context;
	}

	Texture::~Texture(){
		_impl.reset();
	}

	void Texture::initialize(){
		if (!_impl->builder){
			LOGGER->warn("Cannot initialize an already initialized texture");
			return;
		}

		LOGGER->info("Initializing texture...");
		_impl->image = std::make_shared<Internal::Graphics::Image>(GRAPHICS_CONTEXT, _impl->builder->info);

		_impl->transfertBuildDataToInfo();
		_impl->builder.reset();

		LOGGER->info("Texture initialized with success !");
	}

	void Texture::release(){
		_impl->image.reset();
		_impl->builder = std::make_unique<Impl::Builder>();
	}

	void Texture::setFormat(const Format& format){
		_impl->builder->info.format = FormatToVulkan(format);
	}

	void Texture::setWidth(const std::uint32_t& width){
		_impl->builder->info.extent.width = width;
	}

	void Texture::setHeight(const std::uint32_t& height){
		_impl->builder->info.extent.height = height;
	}

	void Texture::setDepth(const std::uint32_t& depth){
		_impl->builder->info.extent.depth = depth;
	}

	void Texture::setUsage(const Usage& usage){
		_impl->builder->info.usage = TextureUsageToVulkan(usage);
	}

	void Texture::setLayout(const Layout& layout){
		_impl->builder->info.initialLayout = TextureLayoutToVulkan(layout);
	}

	void Texture::setTiling(const Tiling& tiling){
		_impl->builder->info.tiling = TextureTilingToVulkan(tiling);
	}

	void Texture::setType(const Type& type){
		_impl->builder->info.imageType = TextureTypeToVulkan(type);
	}

	void Texture::setMipmapCount(const std::uint32_t mip){
		_impl->builder->info.mipLevels = mip;
	}

	void Texture::setArrayLayers(const std::uint32_t layers){
		_impl->builder->info.arrayLayers = layers;
	}

	void Texture::setFlags(const Flags& flags){
		_impl->builder->info.flags = TextureFlagsToVulkan(flags);
	}

	Format Texture::getFormat() const noexcept{
		return _impl->info.format;
	}

	std::uint32_t Texture::getWidth() const noexcept{
		return _impl->info.width;
	}

	std::uint32_t Texture::getHeight() const noexcept{
		return _impl->info.height;
	}

	std::uint32_t Texture::getDepth() const noexcept{
		return _impl->info.depth;
	}

	Texture::Usage Texture::getUsage() const noexcept{
		return _impl->info.usage;
	}

	Texture::Layout Texture::getLayout() const noexcept{
		return _impl->info.layout;
	}

	Texture::Tiling Texture::getTiling() const noexcept{
		return _impl->info.tiling;
	}

	Texture::Type Texture::getType() const noexcept{
		return _impl->info.type;
	}

	std::uint32_t Texture::getMipmapCount() const noexcept{
		return _impl->info.mipCount;
	}

	std::uint32_t Texture::getArrayLayers() const noexcept{
		return _impl->info.arrLayers;
	}

	Texture::Flags Texture::getFlags() const noexcept{
		return _impl->info.flags;
	}

	bool Texture::isInitialized() const noexcept{
		return _impl->image != nullptr;
	}

	void* Texture::getNativeHandle() const{
		if (isInitialized()){
			return _impl->image->get();
		} else {
			spdlog::error("Cannot access the native handle of a non initialied texture");
			throw std::runtime_error("The texture is not initialized");
		}
	}

	Texture::Impl* Texture::getImpl() const noexcept{
		return _impl.get();
	}

	GUID Texture::getGUID() const noexcept{
		
	}


	Texture::FormatProperties Texture::getFormatProperties(const Format& format) const{
		const auto& physicalDevice = _impl->context->getInternalContext()->getRenderer().getContext().getPhysicalDevice();
		VkFormat vkFormat = static_cast<VkFormat>(format.get());

		VkImageFormatProperties vkProperties;

		VkResult result = vkGetPhysicalDeviceImageFormatProperties(
			physicalDevice.get(),
			vkFormat, 
			_impl->builder->info.imageType,
			_impl->builder->info.tiling,
			_impl->builder->info.usage,
			_impl->builder->info.flags,
			&vkProperties
		);

		if (result == VK_ERROR_FORMAT_NOT_SUPPORTED){
			return FormatProperties::UNSUPPORTED;
		}

		if (result != VK_SUCCESS){
			LOGGER->error("Failed to query image format properties {}", string_VkResult(result));
			throw Exceptions::VulkanResourceOperationException(result, "Failed to query image format properties", Exceptions::VulkanOperationType::QUERYING, VK_OBJECT_TYPE_PHYSICAL_DEVICE);
		}

		FormatProperties properties;
		properties.supported = true;
		properties.maxWidth = vkProperties.maxExtent.width;
		properties.maxHeight = vkProperties.maxExtent.height;
		properties.maxDepth = vkProperties.maxExtent.depth;
		properties.maxMipmapLevels = vkProperties.maxMipLevels;
		properties.maxArrayLayers = vkProperties.maxArrayLayers;

		return properties;
	}
	
	std::list<Format> Texture::findAllSupportedFormats(const Format::Properties& requiredProperties, const Format::Features& requiredFeatures, const Format::Properties& except) const{
		std::list<Format> formats;
		if (_impl->builder->info.tiling == VK_IMAGE_TILING_OPTIMAL){
			formats = Format::FindAllTilingOptimal(*_impl->context, requiredProperties, requiredFeatures, except);
		} else {
			formats = Format::FindAllTilingLinear(*_impl->context, requiredProperties, requiredFeatures, except);
		}

		formats.remove_if(
			[&](const Format& format) -> bool {
				auto properties = getFormatProperties(format);
				bool remove = false;

				remove |= !properties.supported;
				remove |= (_impl->builder->info.extent.width > properties.maxWidth);
				remove |= (_impl->builder->info.extent.height > properties.maxHeight);
				remove |= (_impl->builder->info.extent.depth > properties.maxDepth);
				remove |= (_impl->builder->info.mipLevels > properties.maxMipmapLevels);
				remove |= (_impl->builder->info.arrayLayers > properties.maxArrayLayers);

				return remove;
			}
		);

		return formats;
	}

	Format Texture::findBestFormat(const Format::Properties& requiredProperties, const Format::Features& requiredFeatures, const Format::Properties& except) const{

		std::list<std::pair<Format, std::uint32_t>> formats;

		{
			auto source = findAllSupportedFormats(requiredProperties, requiredFeatures, except);
			if (source.empty()){
				LOGGER->warn("Cannot find a single supported format");
				return Format::UNDEFINED;
			}

			std::transform(
				source.begin(),
				source.end(),
				std::back_inserter(formats),
				[](const Format& format) {
					return std::make_pair(format, INFINITY);
				});
		}

		for (auto& pair : formats){
			auto& format = pair.first;
			auto& score = pair.second;

			score = std::__popcount(format.getProperties().get()) - std::__popcount(requiredFeatures.get());
		}

		auto it = std::min_element(
			formats.begin(),
			formats.end(),
			[](const std::pair<Format, std::uint32_t>& a, const std::pair<Format, std::uint32_t>& b) -> bool {
				return a.second < b.second;
			}
		);

		return it->first;
	}

}