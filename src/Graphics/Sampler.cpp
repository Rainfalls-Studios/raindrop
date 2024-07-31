#include <Raindrop/Graphics/Sampler.hpp>
#include <Raindrop/Graphics/Context.hpp>
#include <Raindrop/Exceptions/VulkanExceptions.hpp>

namespace Raindrop::Graphics{
	Sampler::BuildInfo::BuildInfo() : 
		info{
			.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO,
			.pNext = nullptr,
			.flags = 0,
			.magFilter = VK_FILTER_NEAREST,
			.minFilter = VK_FILTER_NEAREST,
			.mipmapMode = VK_SAMPLER_MIPMAP_MODE_NEAREST,
			.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE,
			.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE,
			.addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE,
			.mipLodBias = 1.f,
			.anisotropyEnable = VK_FALSE,
			.maxAnisotropy = 0.f,
			.compareEnable = VK_FALSE,
			.compareOp = VK_COMPARE_OP_NEVER,
			.minLod = 0.f,
			.maxLod = 1.f,
			.unnormalizedCoordinates = VK_FALSE
		}
	{}

	Sampler::Sampler() noexcept : 
		_context{nullptr},
		_sampler{VK_NULL_HANDLE},
		_info{}
	{}

	Sampler::~Sampler(){
		release();
	}

	Sampler::Sampler(Sampler&& other) : 
		_context{nullptr},
		_sampler{VK_NULL_HANDLE},
		_info{}
	{
		swap(*this, other);
	}

	Sampler& Sampler::operator=(Sampler&& other){
		swap(*this, other);
		return *this;
	}

	void swap(Sampler& A, Sampler& B){
		std::swap(A._context, B._context);
		std::swap(A._sampler, B._sampler);
		std::swap(A._info, B._info);
	}

	void Sampler::prepare(Context& context){
		_context = &context;
		_info = std::make_unique<BuildInfo>();
	}

	void Sampler::initialize(){
		if (!_context || !_info){
			(_context == nullptr ? spdlog::default_logger() : _context->logger)->warn("Attempt to initialized a non prepared sampler");
			throw std::runtime_error("The sampler has not been prepared !");
		}

		VkSamplerCreateInfo& info = _info->info;

		auto& device = _context->getDevice();
		auto& allocationCallbacks = _context->core.allocationCallbacks;

		_context->logger->info("Creating sampler ...");

		Exceptions::checkVkCreation<VkSampler>(
			vkCreateSampler(device.get(), &info, allocationCallbacks, &_sampler),
			"Failed to create sampler",
			_context->logger
		);
		
		_info.reset();
	}

	void Sampler::release(){
		if (!_context) return;

		auto& device = _context->getDevice();
		auto& allocationCallbacks = _context->core.allocationCallbacks;

		if (_sampler){
			_context->logger->trace("Destroying sampler ...");

			vkDestroySampler(device.get(), _sampler, allocationCallbacks),
			_sampler = VK_NULL_HANDLE;
		}

		_context = nullptr;
		_info.reset();
	}

	Sampler& Sampler::setFlags(const VkSamplerCreateFlags& flags){
		getInfo().info.flags = flags;
		return *this;
	}

	Sampler& Sampler::setMagFilter(const VkFilter& filter){
		getInfo().info.magFilter = filter;
		return *this;
	}

	Sampler& Sampler::setMinFilter(const VkFilter& filter){
		getInfo().info.minFilter = filter;
		return *this;
	}

	Sampler& Sampler::setMipmapMode(const VkSamplerMipmapMode mode){
		getInfo().info.mipmapMode = mode;
		return *this;
	}

	Sampler& Sampler::setAddressModeU(const VkSamplerAddressMode& mode){
		getInfo().info.addressModeU = mode;
		return *this;
	}

	Sampler& Sampler::setAddressModeV(const VkSamplerAddressMode& mode){
		getInfo().info.addressModeV = mode;
		return *this;
	}

	Sampler& Sampler::setAddressModeW(const VkSamplerAddressMode& mode){
		getInfo().info.addressModeW = mode;
		return *this;
	}

	Sampler& Sampler::setMipLodBias(const float& bias){
		getInfo().info.mipLodBias = bias;
		return *this;
	}

	Sampler& Sampler::enableAnistotropy(const bool& enable){
		getInfo().info.anisotropyEnable = static_cast<VkBool32>(enable);
		return *this;
	}

	Sampler& Sampler::setMaxAnisotropy(const float& max){
		getInfo().info.maxAnisotropy = max;
		return *this;
	}

	Sampler& Sampler::enbaleComparation(const bool& enable){
		getInfo().info.compareEnable = static_cast<VkBool32>(enable);
		return *this;
	}

	Sampler& Sampler::setCompareOperation(const VkCompareOp& operation){
		getInfo().info.compareOp = operation;
		return *this;
	}

	Sampler& Sampler::setMinLod(const float& min){
		getInfo().info.minLod = min;
		return *this;
	}

	Sampler& Sampler::setMaxLod(const float& max){
		getInfo().info.maxLod = max;
		return *this;
	}

	Sampler& Sampler::setBorderColor(const VkBorderColor& color){
		getInfo().info.borderColor = color;
		return *this;
	}

	Sampler& Sampler::unormalizedCoordonates(const bool& unormalize){
		getInfo().info.unnormalizedCoordinates = static_cast<VkBool32>(unormalize);
		return *this;
	}

	const VkSampler& Sampler::get() const noexcept{
		return _sampler;
	}

	Sampler::BuildInfo& Sampler::getInfo(){
		if (!_context || !_info){
			(_context == nullptr ? spdlog::default_logger() : _context->logger)->warn("Attempt to access a non prepared sampler");
			throw std::runtime_error("The sampler has not been prepared !");
		}

		return *_info;
	}

}