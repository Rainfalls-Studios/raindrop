#ifndef __RAINDROP_GRAPHICS_SAMPLER_HPP__
#define __RAINDROP_GRAPHICS_SAMPLER_HPP__

#include "types.hpp"
#include "pch.pch"

namespace Raindrop::Graphics{
	class Sampler{
		public:
			Sampler() noexcept;
			~Sampler();

			Sampler(const Sampler&) = delete;
			Sampler& operator=(const Sampler&) = delete;

			Sampler(Sampler&& other);
			Sampler& operator=(Sampler&& other);

			friend void swap(Sampler& A, Sampler& B);

			void prepare(Context& context);
			void initialize();
			void release();

			Sampler& setFlags(const VkSamplerCreateFlags& flags);
			Sampler& setMagFilter(const VkFilter& filter);
			Sampler& setMinFilter(const VkFilter& filter);
			Sampler& setMipmapMode(const VkSamplerMipmapMode mode);
			Sampler& setAddressModeU(const VkSamplerAddressMode& mode);
			Sampler& setAddressModeV(const VkSamplerAddressMode& mode);
			Sampler& setAddressModeW(const VkSamplerAddressMode& mode);
			Sampler& setMipLodBias(const float& bias);
			Sampler& enableAnistotropy(const bool& enable = true);
			Sampler& setMaxAnisotropy(const float& max);
			Sampler& enbaleComparation(const bool& enable = true);
			Sampler& setCompareOperation(const VkCompareOp& operation);
			Sampler& setMinLod(const float& min);
			Sampler& setMaxLod(const float& max);
			Sampler& setBorderColor(const VkBorderColor& color);
			Sampler& unormalizedCoordonates(const bool& unormalize = true);

			const VkSampler& get() const noexcept;

		private:
			struct BuildInfo{
				VkSamplerCreateInfo info;

				BuildInfo();
			};

			Context* _context;
			VkSampler _sampler;

			std::unique_ptr<BuildInfo> _info;
			BuildInfo& getInfo();

	};
}

#endif