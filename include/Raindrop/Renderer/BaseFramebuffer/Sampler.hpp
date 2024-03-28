#ifndef __RAINDROP_RENDERER_BASE_FRAMEBUFFER_SAMPLER_HPP__
#define __RAINDROP_RENDERER_BASE_FRAMEBUFFER_SAMPLER_HPP__

#include "common.hpp"

namespace Raindrop::Renderer::BaseFramebuffer{
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