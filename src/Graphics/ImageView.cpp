#include <Raindrop/Graphics/ImageView.hpp>
#include <Raindrop/Graphics/Image.hpp>
#include <Raindrop/Graphics/Context.hpp>
#include <Raindrop/Exceptions/VulkanExceptions.hpp>
#include <Raindrop/Context.hpp>

namespace Raindrop::Graphics{
	std::shared_ptr<ImageView> ImageView::create(Raindrop::Context& context){
		return context.registry.emplace<ImageView>();
	}

	ImageView::BuildInfo::BuildInfo() : 
		source{VK_NULL_HANDLE},
		flags{0},
		viewType{VK_IMAGE_VIEW_TYPE_2D},
		format{VK_FORMAT_UNDEFINED},
		componentMapping{
			.r = VK_COMPONENT_SWIZZLE_R,
			.g = VK_COMPONENT_SWIZZLE_G,
			.b = VK_COMPONENT_SWIZZLE_B,
			.a = VK_COMPONENT_SWIZZLE_A,
		},
		imageAspectMask{VK_IMAGE_ASPECT_COLOR_BIT},
		baseMip{0},
		mipCount{1},
		baseLayer{0},
		layerCount{1}
	{}

	ImageView::ImageView() noexcept : 
		_context{nullptr},
		_imageView{VK_NULL_HANDLE},
		_source{},
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
		std::swap(A._source, B._source);
		std::swap(A._info, B._info);
	}

	ImageView& ImageView::prepare(Context& context){
		_context = &context;
		_info = std::make_unique<BuildInfo>();

		return *this;
	}

	void ImageView::initialize(){
		BuildInfo& info = getInfo();

		_source = info.source;

		VkImageViewCreateInfo createInfo{
			.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
			.pNext = nullptr,
			.flags = info.flags,
			.image = _source->get(),
			.viewType = info.viewType,
			.format = info.format,
			.components = info.componentMapping,
			.subresourceRange = {
				.aspectMask = info.imageAspectMask,
				.baseMipLevel = info.baseMip,
				.levelCount = info.mipCount,
				.baseArrayLayer = info.baseLayer,
				.layerCount = info.layerCount
			}
		};

		auto& device = _context->getDevice();
		auto& allocationCallbacks = _context->core.allocationCallbacks;

		Exceptions::checkVkCreation<VkImageView>(
			vkCreateImageView(device.get(), &createInfo, allocationCallbacks, &_imageView),
			"Failed to create image view",
			_context->logger
		);

		_info.reset();
	}

	void ImageView::release(){
		if (!_context) return;

		auto& device = _context->getDevice();
		auto& allocationCallbacks = _context->core.allocationCallbacks;

		_source.reset();

		if (_imageView){
			vkDestroyImageView(device.get(), _imageView, allocationCallbacks);
			_imageView = VK_NULL_HANDLE;
		}

		_context = nullptr;
		_info.reset();
	}

	ImageView& ImageView::setSource(const std::shared_ptr<Image>& image){
		getInfo().source = image;
		return *this;
	}

	ImageView& ImageView::setFlags(const VkImageViewCreateFlags& flags){
		getInfo().flags = flags;
		return *this;
	}

	ImageView& ImageView::setType(const VkImageViewType& type){
		getInfo().viewType = type;
		return *this;
	}

	ImageView& ImageView::setFormat(const VkFormat& format){
		getInfo().format = format;
		return *this;
	}

	ImageView& ImageView::setComponentMapping(const VkComponentMapping& mapping){
		getInfo().componentMapping = mapping;
		return *this;
	}

	ImageView& ImageView::setAspectMask(const VkImageAspectFlags& mask){
		getInfo().imageAspectMask = mask;
		return *this;
	}

	ImageView& ImageView::setBaseMipmapLevel(const std::uint32_t& level){
		getInfo().baseMip = level;
		return *this;
	}

	ImageView& ImageView::setMipmapLevelCount(const std::uint32_t& count){
		getInfo().mipCount = count;
		return *this;
	}

	ImageView& ImageView::setBaseArrayLayer(const std::uint32_t& layer){
		getInfo().baseLayer = layer;
		return *this;
	}

	ImageView& ImageView::setLayerCount(const std::uint32_t& count){
		getInfo().layerCount = count;
		return *this;
	}

	const VkImageView& ImageView::get() const noexcept{
		return _imageView;
	}

	ImageView::BuildInfo& ImageView::getInfo(){
		if (!_context || !_info){
			(_context == nullptr ? spdlog::default_logger() : _context->logger)->warn("Attempt to access build info of a non prepared image view");
			throw std::runtime_error("The image view has not been prepared !");
		}

		return *_info;
	}
}