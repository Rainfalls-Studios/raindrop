#ifndef __RAINDROP_GRAPHICS_PIPELINES_HPP__
#define __RAINDROP_GRAPHICS_PIPELINES_HPP__

#include <Raindrop/Graphics/common.hpp>
#include <Raindrop/Graphics/ShadowMap/Sun/Pipeline.hpp>

namespace Raindrop::Graphics{
	struct Pipelines{
		Pipelines(GraphicsContext& context);
		~Pipelines();

		ShadowMap::Sun::Pipeline sun;

		private:
			GraphicsContext& _context;
	};
}

#endif