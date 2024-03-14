#ifndef __RAINDROP_RENDERER_SCENE_SAMPLER_HPP__
#define __RAINDROP_RENDERER_SCENE_SAMPLER_HPP__

#include "common.hpp"

namespace Raindrop::Renderer::Scene{
	class Sampler{
		public:
			Sampler(Context& context);
			~Sampler();

		private:
			Context& _context;
			VkSampler _sampler;

			void createSampler();
			void destroySampler();
	};
}

#endif