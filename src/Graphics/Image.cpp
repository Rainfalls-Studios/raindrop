#include <Raindrop/Graphics/Image.hpp>
#include <Raindrop/Graphics/Context.hpp>
#include <Raindrop/Exceptions/VulkanExceptions.hpp>
#include <Raindrop/Graphics/Memory.hpp>
#include <Raindrop/Context.hpp>

namespace Raindrop::Graphics{
	std::shared_ptr<Image> Image::create(Raindrop::Context& context){
		return context.registry.emplace<Image>();
	}

	Image::BuildInfo::BuildInfo() : 
		info{
			.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
			.pNext = nullptr,
			.flags = 0,
			.imageType = VK_IMAGE_TYPE_2D,
			.format = VK_FORMAT_UNDEFINED,
			.extent{
				.width = 1,
				.height = 1,
				.depth = 1
			},
			.mipLevels = 1,
			.arrayLayers = 1,
			.samples = VK_SAMPLE_COUNT_1_BIT,
			.tiling = VK_IMAGE_TILING_OPTIMAL,
			.usage = 0,
			.sharingMode = VK_SHARING_MODE_EXCLUSIVE,
			.queueFamilyIndexCount = 1,
			.pQueueFamilyIndices = nullptr,
			.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED
		},
		familyIndices{},
		memory{},
		memoryOffset{0}
	{}

	Image::Image() noexcept : 
		_context{nullptr},
		_image{VK_NULL_HANDLE},
		_memory{},
		_info{}
	{}

	Image::~Image(){
		release();
	}

	Image::Image(Image&& other) : 
		_context{nullptr},
		_image{VK_NULL_HANDLE},
		_memory{},
		_info{}
	{
		swap(*this, other);
	}

	Image& Image::operator=(Image&& other){
		swap(*this, other);
		return *this;
	}

	void swap(Image& A, Image& B){
		std::swap(A._context, B._context);
		std::swap(A._image, B._image);
		std::swap(A._memory, B._memory);
		std::swap(A._info, B._info);
	}

	Image& Image::prepare(Context& context){
		_context = &context;
		_info = std::make_unique<BuildInfo>();

		return *this;
	}

	void Image::initialize(){
		if (!_context || !_info){
			(_context == nullptr ? spdlog::default_logger() : _context->logger)->warn("Attempt to initialized a non prepared image");
			throw std::runtime_error("The image has not been prepared !");
		}

		VkImageCreateInfo& info = _info->info;

		auto& device = _context->getDevice();
		auto& allocationCallbacks = _context->core.allocationCallbacks;

		Exceptions::checkVkCreation<VkImage>(
			vkCreateImage(device.get(), &info, allocationCallbacks, &_image),
			"Failed to create image",
			_context->logger
		);


		if (!_info->memory){
			VkMemoryRequirements memRequirements = getMemoryRequirements();

			_memory = std::make_shared<Memory>();
			_memory->prepare(*_context);
			_memory->setSize(memRequirements.size),
			_memory->setMemoryIndex(device.findMemoryType(memRequirements.memoryTypeBits, _info->memoryProperties));
			_memory->initialize();
		} else {
			_memory = _info->memory;
		}

		Exceptions::checkVkOperation<VkDeviceMemory>(
			vkBindImageMemory(device.get(), _image, _memory->get(), static_cast<VkDeviceSize>(_info->memoryOffset)),
			"Failed to bind image memory",
			Exceptions::VulkanOperationType::BINDING,
			_context->logger
		);

		_info.reset();
	}

	void Image::release(){
		if (!_context) return;

		auto& device = _context->getDevice();
		auto& allocationCallbacks = _context->core.allocationCallbacks;

		_memory.reset();

		if (_image){
			vkDestroyImage(device.get(), _image, allocationCallbacks);
			_image = VK_NULL_HANDLE;
		}

		_context = nullptr;
		_info.reset();
	}

	const VkImage& Image::get() const noexcept{
		return _image;
	}

	Image::BuildInfo& Image::getInfo(){
		if (!_context || !_info){
			(_context == nullptr ? spdlog::default_logger() : _context->logger)->warn("Attempt to access a non prepared image");
			throw std::runtime_error("The image has not been prepared !");
		}

		return *_info;
	}

	VkMemoryRequirements Image::getMemoryRequirements() const{
		checkInitialize();

		auto& device = _context->getDevice();

		VkMemoryRequirements requirements;
		vkGetImageMemoryRequirements(device.get(), _image, &requirements);

		return requirements;
	}

	void Image::checkInitialize() const{
		if (!_context || !_info || _image == VK_NULL_HANDLE){
			(_context == nullptr ? spdlog::default_logger() : _context->logger)->warn("Attempt to access a non initialized image");
			throw std::runtime_error("The image has not been initialized !");
		}
	}

	Image& Image::setFlags(const VkImageCreateFlags& flags){
		getInfo().info.flags = flags;
		return *this;
	}

	Image& Image::setType(const VkImageType& type){
		getInfo().info.imageType = type;
		return *this;
	}

	Image& Image::setFormat(const VkFormat& format){
		getInfo().info.format = format;
		return *this;
	}

	Image& Image::setExtent(const VkExtent3D& extent){
		getInfo().info.extent = extent;
		return *this;
	}

	Image& Image::setWidth(const std::uint32_t& width){
		getInfo().info.extent.width = static_cast<uint32_t>(width);
		return *this;
	}

	Image& Image::setHeight(const std::uint32_t& height){
		getInfo().info.extent.height = static_cast<uint32_t>(height);
		return *this;
	}

	Image& Image::setDepth(const std::uint32_t& depth){
		getInfo().info.extent.depth = static_cast<uint32_t>(depth);
		return *this;
	}

	Image& Image::setExtent(const std::uint32_t& width, const std::uint32_t &height, const std::uint32_t& depth){
		return setExtent(VkExtent3D{width, height, depth});
	}

	Image& Image::setMipmapLevels(const std::uint32_t& levels){
		getInfo().info.mipLevels = levels;
		return *this;
	}

	Image& Image::setArrayLayers(const std::uint32_t& layers){
		getInfo().info.arrayLayers = layers;
		return *this;
	}

	Image& Image::setSampleCount(const VkSampleCountFlagBits& samples){
		getInfo().info.samples = samples;
		return *this;
	}

	Image& Image::setTiling(const VkImageTiling& tiling){
		getInfo().info.tiling = tiling;
		return *this;
	}

	Image& Image::setUsage(const VkImageUsageFlags& usage){
		getInfo().info.usage = usage;
		return *this;
	}

	Image& Image::setSharingMode(const VkSharingMode& mode){
		getInfo().info.sharingMode = mode;
		return *this;
	}

	Image& Image::setQueueFamilies(const std::vector<uint32_t> families){
		auto& info = getInfo();

		info.familyIndices = families;
		info.info.pQueueFamilyIndices = info.familyIndices.data();
		info.info.queueFamilyIndexCount = static_cast<uint32_t>(info.familyIndices.size());

		return *this;
	}

	Image& Image::setInitialLayout(const VkImageLayout& layout){
		getInfo().info.initialLayout = layout;
		return *this;
	}

	Image& Image::setMemoryProperties(const VkMemoryPropertyFlags& properties){
		getInfo().memoryProperties = properties;
		return *this;
	}

	Image& Image::setMemory(const std::shared_ptr<Memory>& memory){
		getInfo().memory = memory;
		return *this;
	}

	Image& Image::setMemoryOffset(const std::size_t& offset){
		getInfo().memoryOffset = offset;
		return *this;
	}

	const std::shared_ptr<Memory> Image::getMemory() const noexcept{
		return _memory;
	}
}