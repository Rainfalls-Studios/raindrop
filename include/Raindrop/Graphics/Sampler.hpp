#ifndef __RAINDROP_GRAPHICS_SAMPLER_HPP__
#define __RAINDROP_GRAPHICS_SAMPLER_HPP__

#include "common.hpp"

namespace Raindrop::Graphics{
	struct SamplerConfigInfo{
		VkFilter magFilter;
		VkFilter minFilter;
		VkSamplerMipmapMode mipmapMode;
		VkSamplerAddressMode addressModeU;
		VkSamplerAddressMode addressModeV;
		VkSamplerAddressMode addressModeW;
		VkCompareOp compareOp;
		VkBorderColor borderColor;
		float mipLodBias;
		float maxAnisotropy;
		float minLod;
		float maxLod;
		bool anisotropyEnable;
		bool compareEnable;
		VkSamplerCreateFlags flags;
	};
	class Sampler{
		public:
			Sampler(Context& context, const SamplerConfigInfo& config);
			~Sampler();

			VkSampler get() const;

		private:
			Context& _context;
			VkSampler _sampler;
	};
}

#endif