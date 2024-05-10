#include <Raindrop/Texture.hpp>
#include <Raindrop/GUID.hpp>
#include <Raindrop_internal/Graphics/Image.hpp>
#include <Raindrop_internal/Context.hpp>
#include <Raindrop_internal/Graphics/Renderer.hpp>
#include <Raindrop_internal/Texture.hpp>
#include <Raindrop/Context.hpp>
#include <Raindrop/Exceptions/VulkanExceptions.hpp>
#include <vulkan/vk_enum_string_helper.h>

#include <bit>

namespace Raindrop{

	const Texture::Operation Texture::Operation::DEFAULT = Texture::Operation(
		Texture::Operation::LOAD,
		Texture::Operation::STORE,
		Texture::Operation::DONT_LOAD,
		Texture::Operation::DONT_STORE
	);

	Texture::Operation::Operation(Load loadOp, Store storeOp, Load stencilLoadOp, Store stentilStorOp) noexcept : 
		_loadOp{loadOp},
		_storeOp{storeOp},
		_stencilLoadOp{stencilLoadOp},
		_stencilStoreOp{stentilStorOp}
	{}

	const Texture::Operation::Load& Texture::Operation::getLoadOp() const noexcept{
		return _loadOp;
	}

	const Texture::Operation::Store& Texture::Operation::getStoreOp() const noexcept{
		return _storeOp;
	}

	const Texture::Operation::Load& Texture::Operation::getStencilLoadOp() const noexcept{
		return _stencilLoadOp;
	}

	const Texture::Operation::Store& Texture::Operation::getStencilStoreOp() const noexcept{
		return _stencilStoreOp;
	}

	void Texture::Operation::setLoadOp(const Load& op) noexcept{
		_loadOp = op;;
	}

	void Texture::Operation::setStoreOp(const Store& op) noexcept{
		_storeOp = op;;
	}

	void Texture::Operation::setStencilLoadOp(const Load& op) noexcept{
		_stencilLoadOp = op;;
	}

	void Texture::Operation::setStencilStoreOp(const Store& op) noexcept{
		_stencilStoreOp = op;;
	}

	//--------------------------------------------------------------------
	//-----------------           TEXTURE                -----------------
	//--------------------------------------------------------------------
	
	static inline std::runtime_error access_error(){
		spdlog::error("Cannot access data of a non initialized texture");
		return std::runtime_error("The texture is not initialized");
	}

	#define INFO _impl->info

	Texture Texture::Create(Context& context){
		return Texture(context);
	}

	Texture::Texture(Context& context) : _impl{nullptr}{
		_impl = new Impl(context);
	}

	Texture::~Texture(){
		delete _impl;
	}

	Texture::Texture(const Texture& other) : _impl{nullptr}{
		_impl = new Impl(*other._impl->context);
		*_impl = *other._impl;
	}

	Texture& Texture::operator=(const Texture& other){
		*_impl = *other._impl;
		return *this;
	}

	#define GRAPHICS_CONTEXT _impl->context->getInternalContext()->getRenderer().getContext()

	void Texture::initialize(){
		_impl->image = std::make_shared<Internal::Graphics::Image>(GRAPHICS_CONTEXT, _impl->info);
	}

	void Texture::release(){
		_impl->image.reset();
	}

	void Texture::setFormat(const Format& format){
		INFO.format = static_cast<VkFormat>(format.get());
	}

	void Texture::setWidth(const std::size_t& width){
		INFO.extent.width = static_cast<uint32_t>(width);
	}

	void Texture::setHeight(const std::size_t& height){
		INFO.extent.height = static_cast<uint32_t>(height);
	}

	void Texture::setDepth(const std::size_t& depth){
		INFO.extent.depth = static_cast<uint32_t>(depth);
	}

	void Texture::setUsage(const Usage& usage){
		INFO.usage = static_cast<VkImageUsageFlags>(usage.get());
	}

	void Texture::setLayout(const Layout& layout){
		INFO.layout = static_cast<VkImageLayout>(layout);
	}

	void Texture::setTiling(const Tiling& tiling){
		INFO.tiling = static_cast<VkImageTiling>(tiling);
	}

	void Texture::setType(const Type& type){
		INFO.type = static_cast<VkImageType>(type);
	}

	void Texture::setMipmapCount(const std::size_t mip){
		INFO.mipLevels = static_cast<uint32_t>(mip);
	}

	void Texture::setArrayLayers(const std::size_t layers){
		INFO.arrLayers = static_cast<uint32_t>(layers);
	}

	void Texture::setFlags(const Flags& flags){
		INFO.flags = static_cast<VkImageCreateFlags>(flags.get());
	}

	Format Texture::getFormat() const noexcept{
		return static_cast<Format::Type>(INFO.format);
	}

	std::size_t Texture::getWidth() const noexcept{
		return static_cast<std::size_t>(INFO.extent.width);
	}

	std::size_t Texture::getHeight() const noexcept{
		return static_cast<std::size_t>(INFO.extent.height);
	}

	std::size_t Texture::getDepth() const noexcept{
		return static_cast<std::size_t>(INFO.extent.depth);
	}

	Texture::Usage Texture::getUsage() const noexcept{
		return static_cast<Usage>(INFO.usage);
	}

	Texture::Layout Texture::getLayout() const noexcept{
		return static_cast<Layout>(INFO.layout);
	}

	Texture::Tiling Texture::getTiling() const noexcept{
		return static_cast<Tiling>(INFO.tiling);
	}

	Texture::Type Texture::getType() const noexcept{
		return static_cast<Type>(INFO.type);
	}

	std::size_t Texture::getMipmapCount() const noexcept{
		return static_cast<std::size_t>(INFO.mipLevels);
	}

	std::size_t Texture::getArrayLayers() const noexcept{
		return static_cast<std::size_t>(INFO.arrLayers);
	}

	Texture::Flags Texture::getFlags() const noexcept{
		return static_cast<Flags>(INFO.flags);
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
		return _impl;
	}

	GUID Texture::getGUID() const noexcept{
		
	}

	const Texture::FormatProperties Texture::FormatProperties::UNSUPPORTED = Texture::FormatProperties{false, 0, 0, 0, 0, 0, 0};

	Texture::FormatProperties Texture::getFormatProperties(const Format& format) const{
		const auto& physicalDevice = _impl->context->getInternalContext()->getRenderer().getContext().getPhysicalDevice();
		auto& logger = _impl->context->getInternalContext()->getRenderer().getContext().getLogger();
		VkFormat vkFormat = static_cast<VkFormat>(format.get());

		VkImageFormatProperties vkProperties;

		VkResult result = vkGetPhysicalDeviceImageFormatProperties(
			physicalDevice.get(),
			vkFormat, 
			_impl->info.type,
			_impl->info.tiling,
			_impl->info.usage,
			_impl->info.flags,
			&vkProperties
		);

		if (result == VK_ERROR_FORMAT_NOT_SUPPORTED){
			return FormatProperties::UNSUPPORTED;
		}

		if (result != VK_SUCCESS){
			logger.error("Failed to query image format properties {}", string_VkResult(result));
			throw Exceptions::VulkanResourceOperationException(result, "Failed to query image format properties", Exceptions::VulkanOperationType::QUERYING, VK_OBJECT_TYPE_PHYSICAL_DEVICE);
		}

		FormatProperties properties;
		properties.supported = true;
		properties.maxWidth = static_cast<std::size_t>(vkProperties.maxExtent.width);
		properties.maxHeight = static_cast<std::size_t>(vkProperties.maxExtent.height);
		properties.maxDepth = static_cast<std::size_t>(vkProperties.maxExtent.depth);
		properties.maxMipmapLevels = static_cast<std::size_t>(vkProperties.maxMipLevels);
		properties.maxArrayLayers = static_cast<std::size_t>(vkProperties.maxArrayLayers);

		return properties;
	}
	
	std::list<Format> Texture::findAllSupportedFormats(const Format::Properties& requiredProperties, const Format::Features& requiredFeatures, const Format::Properties& except) const{
		std::list<Format> formats;
		if (_impl->info.tiling == VK_IMAGE_TILING_OPTIMAL){
			formats = Format::FindAllTilingOptimal(*_impl->context, requiredProperties, requiredFeatures, except);
		} else {
			formats = Format::FindAllTilingLinear(*_impl->context, requiredProperties, requiredFeatures, except);
		}

		formats.remove_if(
			[&](const Format& format) -> bool {
				auto properties = getFormatProperties(format);
				bool remove = false;

				remove |= !properties.supported;
				remove |= (_impl->info.extent.width > properties.maxWidth);
				remove |= (_impl->info.extent.height > properties.maxHeight);
				remove |= (_impl->info.extent.depth > properties.maxDepth);
				remove |= (_impl->info.mipLevels > properties.maxMipmapLevels);
				remove |= (_impl->info.arrLayers > properties.maxArrayLayers);

				return remove;
			}
		);

		return formats;
	}

	Format Texture::findBestFormat(const Format::Properties& requiredProperties, const Format::Features& requiredFeatures, const Format::Properties& except) const{

		std::list<std::pair<Format, std::size_t>> formats;

		{
			auto source = findAllSupportedFormats(requiredProperties, requiredFeatures, except);
			if (source.empty()){
				_impl->context->getInternalContext()->getLogger()->warn("Cannot find a single supported format");
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
			[](const std::pair<Format, std::size_t>& a, const std::pair<Format, std::size_t>& b) -> bool {
				return a.second < b.second;
			}
		);

		return it->first;
	}

}