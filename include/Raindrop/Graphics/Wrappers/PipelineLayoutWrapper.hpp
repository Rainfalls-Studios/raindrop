#ifndef __RAINDROP_GRAPHICS_WRAPPERS_PIPELINE_LAYOUT_WRAPPER_HPP__
#define __RAINDROP_GRAPHICS_WRAPPERS_PIPELINE_LAYOUT_WRAPPER_HPP__

#include "common.hpp"
#include "../Pipelines/common.hpp"

namespace Raindrop::Graphics::Wrappers{
	class PipelineLayoutWrapper{
		public:
			using LayoutID = Pipelines::LayoutID;
			using Layout = Pipelines::PipelineLayout;

			PipelineLayoutWrapper(Context& context);
			PipelineLayoutWrapper(Context& context, const LayoutID& ID);
			PipelineLayoutWrapper(const PipelineLayoutWrapper& other);

			PipelineLayoutWrapper& operator=(const PipelineLayoutWrapper& other);
			PipelineLayoutWrapper& operator=(const LayoutID& ID);
			
			~PipelineLayoutWrapper();
		
			Layout& get();
			const Layout& get() const;

			VkPipelineLayout layout() const;
			
			LayoutID ID() const;

			Layout* operator->();
			const Layout* operator->() const;

		private:
			Context* _context;
			Pipelines::LayoutID _ID;
	};
}

#endif