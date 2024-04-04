#ifndef __RAINDROP_GRAPHICS_PIPELINES_LAYOUT_REGISTRY_HPP__
#define __RAINDROP_GRAPHICS_PIPELINES_LAYOUT_REGISTRY_HPP__

#include "common.hpp"

namespace Raindrop::Graphics::Pipelines{
	class LayoutRegistry{
		public:
			LayoutRegistry(Context& context);
			~LayoutRegistry();

			LayoutID create(const PipelineLayoutConfigInfo& info);
			void destroy(const LayoutID& ID);

			PipelineLayout* get(const LayoutID& ID);

		private:
			Context& _context;

			std::vector<std::unique_ptr<PipelineLayout>> _layouts;
			std::queue<LayoutID> _freeIDs;
	};
}

#endif