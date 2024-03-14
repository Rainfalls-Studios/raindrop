#include <Raindrop/Renderer/Scene/Sampler.hpp>
#include <Raindrop/Renderer/Scene/Context.hpp>
#include <Raindrop/Renderer/Context.hpp>

#include <spdlog/spdlog.h>

namespace Raindrop::Renderer::Scene{
	
	Sampler::Sampler(Context& context) : 
		_context{context},
		_sampler{VK_NULL_HANDLE}
	{
		spdlog::info("Creating sampler ...");
		createSampler();
	}

	Sampler::~Sampler(){
		spdlog::info("Destroying sampler ...");
		destroySampler();
	}

	void Sampler::createSampler(){
		VkSamplerCreateInfo info{};
		info.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
		info.unnormalizedCoordinates = VK_FALSE;

		info.magFilter = VK_FILTER_NEAREST;
		info.minFilter = VK_FILTER_NEAREST;

		info.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
		info.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
		info.addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
		info.borderColor = VK_BORDER_COLOR_FLOAT_TRANSPARENT_BLACK;

		// It is supposed to be a fullscreen quad
		info.anisotropyEnable = VK_FALSE;

		info.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
		info.mipLodBias = 0.0f;
		info.minLod = 0.0f;
		info.maxLod = 0.0f;

		info.compareEnable = VK_FALSE;
		info.compareOp = VK_COMPARE_OP_ALWAYS;

		auto& device = _context.renderer.device;
		auto& allocationCallbacks = _context.renderer.allocationCallbacks;

		if (vkCreateSampler(device.get(), &info, allocationCallbacks, &_sampler) != VK_SUCCESS){
			spdlog::error("Failed to create sampler");
			throw std::runtime_error("Failed to create sampler");
		}
	}

	void Sampler::destroySampler(){
		auto& device = _context.renderer.device;
		auto& allocationCallbacks = _context.renderer.allocationCallbacks;

		if (_sampler != VK_NULL_HANDLE){
			vkDestroySampler(device.get(), _sampler, allocationCallbacks);
			_sampler = VK_NULL_HANDLE;
		}
	}
}