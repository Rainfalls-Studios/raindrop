#ifndef __RAINDROP_GRAPHICS_WRAPPERS_PIPELINE_LAYOUT_WRAPPER_HPP__
#define __RAINDROP_GRAPHICS_WRAPPERS_PIPELINE_LAYOUT_WRAPPER_HPP__

#include "common.hpp"
#include "../Pipelines/common.hpp"
#include "../../Utils/Wrapper.hpp"

namespace Raindrop::Graphics::Wrappers{
	class PipelineLayoutWrapper : private Utils::Wrapper<Context, Pipelines::PipelineLayout, Pipelines::LayoutID, Pipelines::INVALID_LAYOUT_ID>{
		public:
			using LayoutID = Pipelines::LayoutID;
			using Layout = Pipelines::PipelineLayout;

			using Wrapper::Wrapper;
			using Wrapper::operator=;
			using Wrapper::operator->;
			using Wrapper::ID;

			virtual Layout& get() override;
			virtual const Layout& get() const override;

			VkPipelineLayout layout() const;
	};
}

#endif