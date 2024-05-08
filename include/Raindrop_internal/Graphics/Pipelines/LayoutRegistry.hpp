#ifndef __RAINDROP_INTERNAL_GRAPHICS_PIPELINES_LAYOUT_REGISTRY_HPP__
#define __RAINDROP_INTERNAL_GRAPHICS_PIPELINES_LAYOUT_REGISTRY_HPP__

#include "common.hpp"
#include <Raindrop_internal/Templates/Registry.hpp>

namespace Raindrop::Internal::Graphics::Pipelines{
	class LayoutRegistry : private Templates::Registry<std::unique_ptr<PipelineLayout>, LayoutID>{
		public:
			LayoutRegistry(Context& context);
			~LayoutRegistry();

			LayoutID create(const PipelineLayoutConfigInfo& info);
			void destroy(const LayoutID& ID);

			PipelineLayout* get(const LayoutID& ID);

		private:
			Context& _context;
	};
}

#endif