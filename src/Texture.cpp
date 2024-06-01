#include <Raindrop/Texture.hpp>
#include <Raindrop/GUID.hpp>
#include <Raindrop_internal/Graphics/Image.hpp>
#include <Raindrop_internal/Context.hpp>
#include <Raindrop_internal/Graphics/Renderer.hpp>
#include <Raindrop_internal/Texture.hpp>
#include <Raindrop/Context.hpp>
#include <Raindrop/Exceptions/VulkanExceptions.hpp>
#include <vulkan/vk_enum_string_helper.h>

#define LOGGER _impl->context->getInternalContext()->getLogger()
#define INFO _impl->info
#define GRAPHICS_CONTEXT _impl->context->getInternalContext()->getRenderer().getContext()

namespace Raindrop{
	VkImageCreateFlags toVulkan(const Texture::Flags& flags){
		VkImageCreateFlags out = 0;

		if (flags.has(Texture::Flags::SPARSE_BINDING)) out |= VK_IMAGE_CREATE_SPARSE_BINDING_BIT;
		if (flags.has(Texture::Flags::SPARSE_RESIDENCY)) out |= VK_IMAGE_CREATE_SPARSE_RESIDENCY_BIT;
		if (flags.has(Texture::Flags::SPARSE_ALIASED)) out |= VK_IMAGE_CREATE_SPARSE_ALIASED_BIT;
		if (flags.has(Texture::Flags::MUTABLE_FORMAT)) out |= VK_IMAGE_CREATE_MUTABLE_FORMAT_BIT;
		if (flags.has(Texture::Flags::CUBE_COMPATIBLE)) out |= VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT;
		if (flags.has(Texture::Flags::PLANAR_ARRAY_COMPATIBLE)) out |= VK_IMAGE_CREATE_2D_ARRAY_COMPATIBLE_BIT;
		if (flags.has(Texture::Flags::ALIAS)) out |= VK_IMAGE_CREATE_ALIAS_BIT;
		if (flags.has(Texture::Flags::SPLIT_INSTANCE_BIND_REGIONS)) out |= VK_IMAGE_CREATE_SPLIT_INSTANCE_BIND_REGIONS_BIT;
		if (flags.has(Texture::Flags::BLOCK_TEXEL_VIEW_COMPATIBLE)) out |= VK_IMAGE_CREATE_BLOCK_TEXEL_VIEW_COMPATIBLE_BIT;
		if (flags.has(Texture::Flags::EXTENDED_USAGE)) out |= VK_IMAGE_CREATE_EXTENDED_USAGE_BIT;
		if (flags.has(Texture::Flags::PROTECTED)) out |= VK_IMAGE_CREATE_PROTECTED_BIT;
		if (flags.has(Texture::Flags::DISJOINT)) out |= VK_IMAGE_CREATE_DISJOINT_BIT;

		// VK_NV_corner_sampled_image
		if (flags.has(Texture::Flags::CORNER_SAMPLED)) out |= VK_IMAGE_CREATE_CORNER_SAMPLED_BIT_NV;

		// VK_EXT_sample_locations
		if (flags.has(Texture::Flags::SAMPLE_LOCATIONS_COMPATIBLE_DEPTH)) out |= VK_IMAGE_CREATE_SAMPLE_LOCATIONS_COMPATIBLE_DEPTH_BIT_EXT;

		// VK_EXT_fragment_density_map
		if (flags.has(Texture::Flags::SUBSAMPLED)) out |= VK_IMAGE_CREATE_SUBSAMPLED_BIT_EXT;

		// VK_EXT_descriptor_buffer
		if (flags.has(Texture::Flags::DESCRIPTOR_BUFFER_CAPTURE_REPLAY)) out |= VK_IMAGE_CREATE_DESCRIPTOR_BUFFER_CAPTURE_REPLAY_BIT_EXT;

		// VK_EXT_multisampled_render_to_single_sampled
		if (flags.has(Texture::Flags::MULTISAMPLED_RENDER_TO_SINGLE_SAMPLED)) out |= VK_IMAGE_CREATE_MULTISAMPLED_RENDER_TO_SINGLE_SAMPLED_BIT_EXT;

		// VK_EXT_image_2d_view_of_3d
		if (flags.has(Texture::Flags::PLANAR_VIEW_COMPATIBLE)) out |= VK_IMAGE_CREATE_2D_VIEW_COMPATIBLE_BIT_EXT;

		// VK_QCOM_fragment_density_map_offset
		if (flags.has(Texture::Flags::FRAGMENT_DENSITY_MAP_OFFSET)) out |= VK_IMAGE_CREATE_FRAGMENT_DENSITY_MAP_OFFSET_BIT_QCOM;

		// VK_KHR_video_maintenance1
		if (flags.has(Texture::Flags::VIDEO_PROFILE_INDEPENDENT)) out |= VK_IMAGE_CREATE_VIDEO_PROFILE_INDEPENDENT_BIT_KHR;

		return out;
	}


	VkImageUsageFlags toVulkan(const Texture::Usage& u){
		VkImageUsageFlags out = 0;
		
		if (u.has(Texture::Usage::TRANSFER_SRC)) out |= VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
		if (u.has(Texture::Usage::TRANSFER_DST)) out |= VK_IMAGE_USAGE_TRANSFER_DST_BIT;
		if (u.has(Texture::Usage::SAMPLED)) out |= VK_IMAGE_USAGE_SAMPLED_BIT;
		if (u.has(Texture::Usage::STORAGE)) out |= VK_IMAGE_USAGE_STORAGE_BIT;
		if (u.has(Texture::Usage::COLOR_ATTACHMENT)) out |= VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
		if (u.has(Texture::Usage::DEPTH_STENCIL_ATTACHMENT)) out |= VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
		if (u.has(Texture::Usage::TRANSIENT_ATTACHMENT)) out |= VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT;
		if (u.has(Texture::Usage::INPUT_ATTACHMENT)) out |= VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT;
		
		// VK_KHR_video_decode_queue
		if (u.has(Texture::Usage::VIDEO_DECODE_DST)) out |= VK_IMAGE_USAGE_VIDEO_DECODE_DST_BIT_KHR;
		if (u.has(Texture::Usage::VIDEO_DECODE_SRC)) out |= VK_IMAGE_USAGE_VIDEO_DECODE_SRC_BIT_KHR;
		if (u.has(Texture::Usage::VIDEO_DECODE_DPB)) out |= VK_IMAGE_USAGE_VIDEO_DECODE_DPB_BIT_KHR;
		
		// VK_KHR_video_encode_queue
		if (u.has(Texture::Usage::VIDEO_ENCODE_DST)) out |= VK_IMAGE_USAGE_VIDEO_ENCODE_DST_BIT_KHR;
		if (u.has(Texture::Usage::VIDEO_ENCODE_SRC)) out |= VK_IMAGE_USAGE_VIDEO_ENCODE_SRC_BIT_KHR;
		if (u.has(Texture::Usage::VIDEO_ENCODE_DPB)) out |= VK_IMAGE_USAGE_VIDEO_ENCODE_DPB_BIT_KHR;

		// VK_EXT_fragment_density_map
		if (u.has(Texture::Usage::FRAGMENT_DENSITY_MAP)) out |= VK_IMAGE_USAGE_FRAGMENT_DENSITY_MAP_BIT_EXT;

		// VK_KHR_fragment_shading_rate
		if (u.has(Texture::Usage::FRAGMENT_SHADING_RATE_ATTACHMENT)) out |= VK_IMAGE_USAGE_FRAGMENT_SHADING_RATE_ATTACHMENT_BIT_KHR;

		// VK_EXT_host_image_copy
		if (u.has(Texture::Usage::HOST_TRANSFER)) out |= VK_IMAGE_USAGE_HOST_TRANSFER_BIT_EXT;

		// VK_EXT_attachment_feedback_loop_layout
		if (u.has(Texture::Usage::ATTACHMENT_FEEDBACK_LOOP)) out |= VK_IMAGE_USAGE_ATTACHMENT_FEEDBACK_LOOP_BIT_EXT;

		// VK_HUAWEI_invocation_mask
		if (u.has(Texture::Usage::INVOCATION_MASK)) out |= VK_IMAGE_USAGE_INVOCATION_MASK_BIT_HUAWEI;

		// VK_QCOM_image_processing
		if (u.has(Texture::Usage::SAMPLE_WEIGHT)) out |= VK_IMAGE_USAGE_SAMPLE_WEIGHT_BIT_QCOM;
		if (u.has(Texture::Usage::SAMPLE_BLOCK_MATCH)) out |= VK_IMAGE_USAGE_SAMPLE_BLOCK_MATCH_BIT_QCOM;

		return out;
	}

	VkImageType toVulkan(const Texture::Type& t){
		switch (t){
			case Texture::Type::LINEAR: return VK_IMAGE_TYPE_1D;
			case Texture::Type::PLANAR: return VK_IMAGE_TYPE_2D;
			case Texture::Type::VOLUMETRIC: return VK_IMAGE_TYPE_3D;
		}
	}

	VkImageLayout toVulkan(const Texture::Layout& layout){
		switch (layout){
			case Texture::Layout::UNDEFINED: return VK_IMAGE_LAYOUT_UNDEFINED;
			case Texture::Layout::GENERAL: return VK_IMAGE_LAYOUT_GENERAL;
			case Texture::Layout::COLOR_ATTACHMENT_OPTIMAL: return VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
			case Texture::Layout::DEPTH_STENCIL_ATTACHMENT_OPTIMAL: return VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
			case Texture::Layout::DEPTH_STENCIL_READ_ONLY_OPTIMAL: return VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL;
			case Texture::Layout::SHADER_READ_ONLY_OPTIMAL: return VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			case Texture::Layout::TRANSFER_SRC_OPTIMAL: return VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
			case Texture::Layout::TRANSFER_DST_OPTIMAL: return VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
			case Texture::Layout::PREINITIALIZED: return VK_IMAGE_LAYOUT_PREINITIALIZED;
			case Texture::Layout::DEPTH_READ_ONLY_STENCIL_ATTACHMENT_OPTIMAL: return VK_IMAGE_LAYOUT_DEPTH_READ_ONLY_STENCIL_ATTACHMENT_OPTIMAL;
			case Texture::Layout::DEPTH_ATTACHMENT_STENCIL_READ_ONLY_OPTIMAL: return VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_STENCIL_READ_ONLY_OPTIMAL;
			case Texture::Layout::DEPTH_ATTACHMENT_OPTIMAL: return VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL;
			case Texture::Layout::DEPTH_READ_ONLY_OPTIMAL: return VK_IMAGE_LAYOUT_DEPTH_READ_ONLY_OPTIMAL;
			case Texture::Layout::STENCIL_ATTACHMENT_OPTIMAL: return VK_IMAGE_LAYOUT_STENCIL_ATTACHMENT_OPTIMAL;
			case Texture::Layout::STENCIL_READ_ONLY_OPTIMAL: return VK_IMAGE_LAYOUT_STENCIL_READ_ONLY_OPTIMAL;
			case Texture::Layout::READ_ONLY_OPTIMAL: return VK_IMAGE_LAYOUT_READ_ONLY_OPTIMAL;
			case Texture::Layout::ATTACHMENT_OPTIMAL: return VK_IMAGE_LAYOUT_ATTACHMENT_OPTIMAL;

			// VK_KHR_swapchain
			case Texture::Layout::PRESENT_SRC: return VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

			//VK_KHR_video_decode_queue
			case Texture::Layout::VIDEO_DECODE_DST: return VK_IMAGE_LAYOUT_VIDEO_DECODE_DST_KHR;
			case Texture::Layout::VIDEO_DECODE_SRC: return VK_IMAGE_LAYOUT_VIDEO_DECODE_SRC_KHR;
			case Texture::Layout::VIDEO_DECODE_DPB: return VK_IMAGE_LAYOUT_VIDEO_DECODE_DPB_KHR;

			// VK_KHR_video_encode_queue
			case Texture::Layout::VIDEO_ENCODE_DST: return VK_IMAGE_LAYOUT_VIDEO_ENCODE_DST_KHR;
			case Texture::Layout::VIDEO_ENCODE_SRC: return VK_IMAGE_LAYOUT_VIDEO_ENCODE_SRC_KHR;
			case Texture::Layout::VIDEO_ENCODE_DPB: return VK_IMAGE_LAYOUT_VIDEO_ENCODE_DPB_KHR;

			// VK_KHR_shared_presentable_image
			case Texture::Layout::SHARED_PRESENT: return VK_IMAGE_LAYOUT_SHARED_PRESENT_KHR;

			// VK_EXT_fragment_density_map
			case Texture::Layout::FRAGMENT_DENSITY_MAP_OPTIMAL: return VK_IMAGE_LAYOUT_FRAGMENT_DENSITY_MAP_OPTIMAL_EXT;

			// VK_KHR_fragment_shading_rate
			case Texture::Layout::FRAGMENT_SHADING_RATE_ATTACHMENT_OPTIMAL: return VK_IMAGE_LAYOUT_FRAGMENT_SHADING_RATE_ATTACHMENT_OPTIMAL_KHR;

			// VK_KHR_dynamic_rendering_local_read
			case Texture::Layout::RENDERING_LOCAL_READ: return VK_IMAGE_LAYOUT_RENDERING_LOCAL_READ_KHR;

			// VK_EXT_attachment_feedback_loop_layout
			case Texture::Layout::ATTACHMENT_FEEDBACK_LOOP_OPTIMAL: return VK_IMAGE_LAYOUT_ATTACHMENT_FEEDBACK_LOOP_OPTIMAL_EXT;
		}

		throw std::runtime_error("Invalid layout value");
	}

	VkImageTiling toVulkan(const Texture::Tiling& t){
		switch (t){
			case Texture::Tiling::LINEAR: return VK_IMAGE_TILING_LINEAR;
			case Texture::Tiling::OPTIMAL: return VK_IMAGE_TILING_OPTIMAL;
		}
		throw std::runtime_error("Invalid tiling value");
	}

	VkImageAspectFlags toVulkan(const Texture::Aspect& a){
		VkImageAspectFlags out = VK_IMAGE_ASPECT_NONE;

		if (a.has(Texture::Aspect::COLOR)) out |= VK_IMAGE_ASPECT_COLOR_BIT;
		if (a.has(Texture::Aspect::DEPTH)) out |= VK_IMAGE_ASPECT_DEPTH_BIT;
		if (a.has(Texture::Aspect::STENCIL)) out |= VK_IMAGE_ASPECT_STENCIL_BIT;
		if (a.has(Texture::Aspect::METADATA)) out |= VK_IMAGE_ASPECT_METADATA_BIT;
		if (a.has(Texture::Aspect::PLANE_0)) out |= VK_IMAGE_ASPECT_PLANE_0_BIT;
		if (a.has(Texture::Aspect::PLANE_1)) out |= VK_IMAGE_ASPECT_PLANE_1_BIT;
		if (a.has(Texture::Aspect::PLANE_2)) out |= VK_IMAGE_ASPECT_PLANE_2_BIT;

		// VK_EXT_image_drm_format_modifier
		if (a.has(Texture::Aspect::MEMORY_PLANE_0)) out |= VK_IMAGE_ASPECT_MEMORY_PLANE_0_BIT_EXT;
		if (a.has(Texture::Aspect::MEMORY_PLANE_1)) out |= VK_IMAGE_ASPECT_MEMORY_PLANE_1_BIT_EXT;
		if (a.has(Texture::Aspect::MEMORY_PLANE_2)) out |= VK_IMAGE_ASPECT_MEMORY_PLANE_2_BIT_EXT;
		if (a.has(Texture::Aspect::MEMORY_PLANE_3)) out |= VK_IMAGE_ASPECT_MEMORY_PLANE_3_BIT_EXT;

		return out;
	}


	//--------------------------------------------------------------------
	//-----------------           TEXTURE                -----------------
	//--------------------------------------------------------------------
	
	static inline std::runtime_error access_error(){
		spdlog::error("Cannot access data of a non initialized texture");
		return std::runtime_error("The texture is not initialized");
	}

	const Texture::FormatProperties Texture::FormatProperties::UNSUPPORTED = Texture::FormatProperties{false, 0, 0, 0, 0, 0, 0};

	Texture Texture::Create(Context& context){
		return Texture(context);
	}

	Texture::Texture(Context& context) : _impl{nullptr}{
		_impl = new Impl(context);
	}

	Texture::~Texture(){
		delete _impl;
		_impl = nullptr;
	}

	Texture::Texture(const Texture& other) : _impl{nullptr}{
		_impl = new Impl(*other._impl->context);
		*_impl = *other._impl;
	}

	Texture& Texture::operator=(const Texture& other){
		*_impl = *other._impl;
		return *this;
	}

	void Texture::initialize(){
		LOGGER->info("Initializing texture...");
		_impl->image = std::make_shared<Internal::Graphics::Image>(GRAPHICS_CONTEXT, _impl->info);
		LOGGER->info("Texture initialized with success !");
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


	Texture::FormatProperties Texture::getFormatProperties(const Format& format) const{
		const auto& physicalDevice = _impl->context->getInternalContext()->getRenderer().getContext().getPhysicalDevice();
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
			LOGGER->error("Failed to query image format properties {}", string_VkResult(result));
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
			[](const std::pair<Format, std::size_t>& a, const std::pair<Format, std::size_t>& b) -> bool {
				return a.second < b.second;
			}
		);

		return it->first;
	}

}