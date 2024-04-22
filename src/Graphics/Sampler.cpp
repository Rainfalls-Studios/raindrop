#include <Raindrop/Graphics/Sampler.hpp>
#include <Raindrop/Graphics/Context.hpp>
#include <Raindrop/Exceptions/VulkanExceptions.hpp>

namespace Raindrop::Graphics{
	Sampler::Sampler(Context& context, const SamplerConfigInfo& config) : 
		_context{context},
		_sampler{VK_NULL_HANDLE}
	{

		auto& device = _context.device;
		const auto& allocationCallbacks = _context.allocationCallbacks;
		
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
		auto& device = _context.device;
		const auto& allocationCallbacks = _context.allocationCallbacks;

		if (_sampler != VK_NULL_HANDLE){
			vkDestroySampler(device.get(), _sampler, allocationCallbacks);
			_sampler = VK_NULL_HANDLE;
		}
	}

	VkSampler Sampler::get() const{
		return _sampler;
	}
}