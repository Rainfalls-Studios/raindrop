#ifndef __RAINDROP_GRAPHICS_BASE_FRAMEBUFFER_SAMPLER_HPP__
#define __RAINDROP_GRAPHICS_BASE_FRAMEBUFFER_SAMPLER_HPP__

#include "common.hpp"

namespace Raindrop::Graphics::BaseFramebuffer{
	class Sampler{
		public:
			Sampler(Context& context);
			~Sampler();

			VkSampler get() const;

		private:
			Context& _context;
			VkSampler _sampler;

			void createSampler();
			void destroySampler();
	};
}

#endif