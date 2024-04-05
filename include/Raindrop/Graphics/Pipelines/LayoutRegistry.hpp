#ifndef __RAINDROP_GRAPHICS_PIPELINES_LAYOUT_REGISTRY_HPP__
#define __RAINDROP_GRAPHICS_PIPELINES_LAYOUT_REGISTRY_HPP__

#include "common.hpp"
#include "../../Utils/Registry.hpp"

namespace Raindrop::Graphics::Pipelines{
	class LayoutRegistry : private Utils::Registry<std::unique_ptr<PipelineLayout>, LayoutID>{
		public:
			LayoutRegistry(Context& context);
			~LayoutRegistry();

			LayoutID create(const PipelineLayoutConfigInfo& info);
			void destroy(const LayoutID& ID);

			PipelineLayout* get(const LayoutID& ID);

			using Registry::exists;
			using Registry::inRange;

			using Registry::reservedSize;
			using Registry::freeSize;
			using Registry::usedSize;

		private:
			Context& _context;
	};
}

#endif