#ifndef __RAINDROP_INTERNAL_PIPELINE_HPP__
#define __RAINDROP_INTERNAL_PIPELINE_HPP__

#include <Raindrop/Pipeline.hpp>
#include <Raindrop/Context.hpp>
#include <Raindrop_internal/Graphics/GraphicsPipeline.hpp>
#include <Raindrop_internal/Graphics/PipelineLayout.hpp>
#include <Raindrop_internal/Graphics/Shader.hpp>

namespace Raindrop{
	// TODO: make infos temporary -> destroy on resource initialization
	
	struct Pipeline::Layout::Impl{
		std::shared_ptr<Internal::Graphics::PipelineLayout> layout;
		Internal::Graphics::PipelineLayoutConfigInfo info;
		Context* context;

		Impl(Context& context) : context{&context}{}
	};

	struct Pipeline::Shader::Impl{
		std::shared_ptr<Internal::Graphics::Shader> module;
		Internal::Graphics::ShaderConfigInfo info;
		Context& context;

		Impl(Context& context) : context{context}{}
	};

	struct Pipeline::Impl{
		std::shared_ptr<Internal::Graphics::GraphicsPipeline> pipeline;
		Internal::Graphics::GraphicsPipelineConfigInfo info;
		Context* context;

		Impl(Context& context) : context{&context}{}
	};
}

#endif