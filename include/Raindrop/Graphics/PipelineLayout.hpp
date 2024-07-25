#ifndef __RAINDROP_GRAPHICS_PIPELINE_LAYOUT_HPP__
#define __RAINDROP_GRAPHICS_PIPELINE_LAYOUT_HPP__

#include "types.hpp"
#include "pch.pch"

namespace Raindrop::Graphics{
	class PipelineLayout{
		public:
			PipelineLayout();

		private:
			Context* _context;

	};
}

#endif