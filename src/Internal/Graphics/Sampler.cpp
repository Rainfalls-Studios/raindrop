#include <Raindrop_internal/Graphics/Sampler.hpp>
#include <Raindrop_internal/Graphics/Context.hpp>
#include <Raindrop/Exceptions/VulkanExceptions.hpp>

namespace Raindrop::Internal::Graphics{
	SamplerConfigInfo::SamplerConfigInfo() : 
		magFilter{VK_FILTER_NEAREST},
		minFilter{VK_FILTER_NEAREST},
		addressModeU{VK_SAMPLER_ADDRESS_MODE_REPEAT},
		addressModeV{VK_SAMPLER_ADDRESS_MODE_REPEAT},
		addressModeW{VK_SAMPLER_ADDRESS_MODE_REPEAT},
		borderColor{VK_BORDER_COLOR_FLOAT_TRANSPARENT_BLACK},
		anisotropyEnable{false},
		maxAnisotropy{0},
		mipmapMode{VK_SAMPLER_MIPMAP_MODE_LINEAR},
		mipLodBias{0},
		minLod{0},
		maxLod{0},
		compareEnable{false},
		compareOp{VK_COMPARE_OP_ALWAYS},
		unnormalizedCoordinates{false},
		flags{0}
	{}

	Sampler::Sampler(Context& context, const SamplerConfigInfo& config) : 
		_context{context},
		_sampler{VK_NULL_HANDLE}
	{

		auto& device = _context.getDevice();
		const auto& allocationCallbacks = _context.getAllocationCallbacks();
		
		VkSamplerCreateInfo info{};

		info.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
		info.magFilter = config.magFilter;
		info.minFilter = config.minFilter;

		info.addressModeU = config.addressModeU;
		info.addressModeV = config.addressModeV;
		info.addressModeW = config.addressModeW;

		info.mipmapMode = config.mipmapMode;

		info.compareOp = config.compareOp;
		info.compareEnable = config.compareEnable;

		info.anisotropyEnable = config.anisotropyEnable;
		info.maxAnisotropy = config.maxAnisotropy;
		
		info.borderColor = config.borderColor;
		info.mipLodBias = config.mipLodBias;
		info.minLod = config.minLod;
		info.maxLod = config.maxLod;

		Exceptions::checkVkCreation<VkSampler>(
			vkCreateSampler(device.get(), &info, allocationCallbacks, &_sampler),
			"Failed to create sampler"
		);
	}

	Sampler::~Sampler(){
		auto& device = _context.getDevice();
		const auto& allocationCallbacks = _context.getAllocationCallbacks();

		if (_sampler != VK_NULL_HANDLE){
			vkDestroySampler(device.get(), _sampler, allocationCallbacks);
			_sampler = VK_NULL_HANDLE;
		}
	}

	VkSampler Sampler::get() const{
		return _sampler;
	}
}