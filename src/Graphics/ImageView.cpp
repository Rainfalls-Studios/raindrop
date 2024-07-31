#include <Raindrop/Graphics/ImageView.hpp>
#include <Raindrop/Graphics/Image.hpp>
#include <Raindrop/Graphics/Context.hpp>
#include <Raindrop/Exceptions/VulkanExceptions.hpp>

namespace Raindrop::Graphics{
	ImageView::BuildInfo::BuildInfo() : 
		info{
			.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
			.pNext = nullptr,
			.flags = 0,
			.image = VK_NULL_HANDLE,
			.format = VK_FORMAT_UNDEFINED,
			.components{
				.r = VK_COMPONENT_SWIZZLE_R,
				.g = VK_COMPONENT_SWIZZLE_G,
				.b = VK_COMPONENT_SWIZZLE_B,
				.a = VK_COMPONENT_SWIZZLE_A,
			},
			.subresourceRange{
				.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
				.baseMipLevel = 0,
				.levelCount = 1,
				.baseArrayLayer = 0,
				.layerCount = 1
			}
		}
	{}

	ImageView::ImageView() noexcept : 
		_context{nullptr},
		_imageView{VK_NULL_HANDLE},
		_info{}
	{}

	ImageView::~ImageView(){
		release();
	}

	ImageView::ImageView(ImageView&& other) : 
		_context{nullptr},
		_imageView{VK_NULL_HANDLE},
		_info{}
	{
		swap(*this, other);
	}

	ImageView& ImageView::operator=(ImageView&& other){
		swap(*this, other);
		return *this;
	}

	void swap(ImageView& A, ImageView& B){
		std::swap(A._context, B._context);
		std::swap(A._imageView, B._imageView);
		std::swap(A._info, B._info);
	}

	void ImageView::prepare(Context& context){
		_context = &context;
		_info = std::make_unique<BuildInfo>();
	}

	void ImageView::initialize(){
		if (!_context || !_info){
			(_context == nullptr ? spdlog::default_logger() : _context->logger)->warn("Attempt to initialized a non prepared image view");
			throw std::runtime_error("The image view has not be en prepared !");
		}

		VkImageViewCreateInfo& info = _info->info;

		auto& device = _context->getDevice();
		auto& allocationCallbacks = _context->core.allocationCallbacks;

		Exceptions::checkVkCreation<VkImageView>(
			vkCreateImageView(device.get(), &info, allocationCallbacks, &_imageView),
			"Failed to create image view",
			_context->logger
		);
	}

	void ImageView::release(){

	}

	ImageView& ImageView::setSource(const Image& image){
		getInfo().info.image = image.get();
		return *this;
	}

	ImageView& ImageView::setFlags(const VkImageViewCreateFlags& flags){
		getInfo().info.flags = flags;
		return *this;
	}

	ImageView& ImageView::setSource(const VkImage& image){
		getInfo().info.image = image;
		return *this;
	}

	ImageView& ImageView::setType(const VkImageViewType& type){
		getInfo().info.viewType = type;
		return *this;
	}

	ImageView& ImageView::setFormat(const VkFormat& format){
		getInfo().info.format = format;
		return *this;
	}

	ImageView& ImageView::setComponentMapping(const VkComponentMapping& mapping){
		getInfo().info.components = mapping;
		return *this;
	}

	ImageView& ImageView::setAspectMask(const VkImageAspectFlags& mask){
		getInfo().info.subresourceRange.aspectMask = mask;
		return *this;
	}

	ImageView& ImageView::setBaseMipmapLevel(const std::uint32_t& level){
		getInfo().info.subresourceRange.baseMipLevel = level;
		return *this;
	}

	ImageView& ImageView::setMipmapLevelCount(const std::uint32_t& count){
		getInfo().info.subresourceRange.levelCount = count;
		return *this;
	}

	ImageView& ImageView::setBaseArrayLayer(const std::uint32_t& layer){
		getInfo().info.subresourceRange.baseArrayLayer = layer;
		return *this;
	}

	ImageView& ImageView::setLayerCount(const std::uint32_t& count){
		getInfo().info.subresourceRange.layerCount = count;
		return *this;
	}

	const VkImageView& ImageView::get() const noexcept{
		return _imageView;
	}

	ImageView::BuildInfo& ImageView::getInfo(){
		if (!_context || !_info){
			(_context == nullptr ? spdlog::default_logger() : _context->logger)->warn("Attempt to access a non prepared image view");
			throw std::runtime_error("The image view has not been prepared !");
		}

		return *_info;
	}
}