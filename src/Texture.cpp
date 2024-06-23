#include <Raindrop/Texture.hpp>
#include <Raindrop_internal/Texture.hpp>
#include <Raindrop/GUID.hpp>
#include <Raindrop_internal/Graphics/Image.hpp>
#include <Raindrop_internal/Context.hpp>
#include <Raindrop_internal/Graphics/Engine.hpp>
#include <Raindrop/Context.hpp>
#include <Raindrop/Exceptions/VulkanExceptions.hpp>
#include <Raindrop_internal/Format.hpp>
#include <vulkan/vk_enum_string_helper.h>

#define LOGGER _impl->context->getLogger()
#define GRAPHICS_CONTEXT _impl->context->getEngine().getContext()

namespace Raindrop{
	//--------------------------------------------------------------------
	//-----------------           TEXTURE                -----------------
	//--------------------------------------------------------------------

	const Texture::FormatProperties Texture::FormatProperties::UNSUPPORTED = Texture::FormatProperties{false, 0, 0, 0, 0, 0, 0};

	Texture Texture::Create(Context& context){
		return Texture(context);
	}

	Texture::Texture(std::unique_ptr<Impl>&& impl) : _impl{std::move(impl)}{}

	Texture::Texture(Context& context) : _impl{std::make_unique<Impl>()}{
		_impl->context = context.getInternalContext();
	}

	Texture::~Texture(){
		_impl.reset();
	}

	void Texture::initialize(){
		LOGGER->info("Initializing texture...");

		VkImageCreateInfo info{
			.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
			.pNext = nullptr,
			.flags = TextureFlagsToVulkan(_impl->flags),
			.imageType = TextureTypeToVulkan(_impl->type),
			.format = FormatToVulkan(_impl->format),
			.extent{
				.width = static_cast<uint32_t>(_impl->width),
				.height = static_cast<uint32_t>(_impl->height),
				.depth = static_cast<uint32_t>(_impl->depth)
			},
			.mipLevels = static_cast<uint32_t>(_impl->mipmapCount),
			.arrayLayers = static_cast<uint32_t>(_impl->layerCount),
			.samples = VK_SAMPLE_COUNT_1_BIT,
			.tiling = TextureTilingToVulkan(_impl->tiling),
			.usage = TextureUsageToVulkan(_impl->usage),
			.sharingMode = VK_SHARING_MODE_EXCLUSIVE,
			.queueFamilyIndexCount = 0,
			.pQueueFamilyIndices = nullptr,
			.initialLayout = TextureLayoutToVulkan(_impl->initialLayout)
		};

		_impl->internal = std::make_shared<Internal::Graphics::Image>(GRAPHICS_CONTEXT, info);
		_impl->currentLayout = _impl->initialLayout;

		LOGGER->info("Texture initialized with success !");
	}

	void Texture::release(){
		Internal::Context* context = _impl->context;

		_impl = std::make_unique<Impl>();
		_impl->context = context;
	}

	void Texture::setFormat(const Format& format){
		_impl->format = format;
	}

	void Texture::setWidth(const std::uint32_t& width){
		_impl->width = width;
	}

	void Texture::setHeight(const std::uint32_t& height){
		_impl->height = height;
	}

	void Texture::setDepth(const std::uint32_t& depth){
		_impl->depth = depth;
	}

	void Texture::setUsage(const Usage& usage){
		_impl->usage = usage;
	}

	void Texture::setInitialLayout(const Layout& layout){
		_impl->initialLayout = layout;
	}

	void Texture::setTiling(const Tiling& tiling){
		_impl->tiling = tiling;
	}

	void Texture::setType(const Type& type){
		_impl->type = type;
	}

	void Texture::setMipmapCount(const std::uint32_t mip){
		_impl->mipmapCount = mip;
	}

	void Texture::setArrayLayers(const std::uint32_t layers){
		_impl->layerCount = layers;
	}

	void Texture::setFlags(const Flags& flags){
		_impl->flags = flags;
	}

	const Format& Texture::getFormat() const noexcept{
		return _impl->format;
	}

	const std::uint32_t& Texture::getWidth() const noexcept{
		return _impl->width;
	}

	const std::uint32_t& Texture::getHeight() const noexcept{
		return _impl->height;
	}

	const std::uint32_t& Texture::getDepth() const noexcept{
		return _impl->depth;
	}

	const Texture::Usage& Texture::getUsage() const noexcept{
		return _impl->usage;
	}

	const Texture::Layout& Texture::getInitialLayout() const noexcept{
		return _impl->initialLayout;
	}

	const Texture::Layout& Texture::getCurrentLayout() const noexcept{
		return _impl->currentLayout;
	}

	const Texture::Tiling& Texture::getTiling() const noexcept{
		return _impl->tiling;
	}

	const Texture::Type& Texture::getType() const noexcept{
		return _impl->type;
	}

	const std::uint32_t& Texture::getMipmapCount() const noexcept{
		return _impl->mipmapCount;
	}

	const std::uint32_t& Texture::getArrayLayers() const noexcept{
		return _impl->layerCount;
	}

	const Texture::Flags& Texture::getFlags() const noexcept{
		return _impl->flags;
	}

	bool Texture::isInitialized() const noexcept{
		return _impl->internal != nullptr;
	}

	void* Texture::getNativeHandle() const noexcept{
		return _impl->internal != nullptr ? static_cast<void*>(_impl->internal->get()) : nullptr;
	}

	Texture::Impl* Texture::getImpl() const noexcept{
		return _impl.get();
	}

	GUID Texture::getGUID() const noexcept{
		
	}


	Texture::FormatProperties Texture::getFormatProperties(const Format& format) const{
		const auto& physicalDevice = _impl->context->getEngine().getContext().getPhysicalDevice();
		VkFormat vkFormat =  FormatToVulkan(format);

		VkImageFormatProperties vkProperties{};

		VkResult result = vkGetPhysicalDeviceImageFormatProperties(
			physicalDevice.get(),
			vkFormat, 
			TextureTypeToVulkan(_impl->type),
			TextureTilingToVulkan(_impl->tiling),
			TextureUsageToVulkan(_impl->usage),
			TextureFlagsToVulkan(_impl->flags),
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
		if (_impl->tiling == Tiling::OPTIMAL){
			formats = Format::FindAllTilingOptimal(_impl->context->getInterface(), requiredProperties, requiredFeatures, except);
		} else {
			formats = Format::FindAllTilingLinear(_impl->context->getInterface(), requiredProperties, requiredFeatures, except);
		}

		formats.remove_if(
			[&](const Format& format) -> bool {
				auto properties = getFormatProperties(format);
				bool remove = false;

				remove |= !properties.supported;
				remove |= (_impl->width > properties.maxWidth);
				remove |= (_impl->height > properties.maxHeight);
				remove |= (_impl->depth > properties.maxDepth);
				remove |= (_impl->mipmapCount > properties.maxMipmapLevels);
				remove |= (_impl->layerCount > properties.maxArrayLayers);

				return remove;
			}
		);

		return formats;
	}

	Format Texture::findBestFormat(const Format::Properties& requiredProperties, const Format::Features& requiredFeatures, const Format::Properties& except) const{

		std::list<std::pair<Format, std::uint32_t>> formats;

		// Copy all formats
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

		// asign a score for each of them
		for (auto& pair : formats){
			auto& format = pair.first;
			auto& score = pair.second;

			// TODO: check fiability
			score = std::__popcount(format.getProperties().get()) - std::__popcount(requiredFeatures.get());
		}

		// get the best one
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