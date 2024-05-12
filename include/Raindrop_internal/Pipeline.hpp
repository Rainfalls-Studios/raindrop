#ifndef __RAINDROP_INTERNAL_PIPELINE_HPP__
#define __RAINDROP_INTERNAL_PIPELINE_HPP__

#include <Raindrop/Pipeline.hpp>
#include <Raindrop/Context.hpp>
#include <Raindrop_internal/Graphics/Pipelines/GraphicsPipeline.hpp>
#include <Raindrop_internal/Graphics/Pipelines/PipelineLayout.hpp>
#include <Raindrop_internal/Graphics/Shader.hpp>

namespace Raindrop{
	// TODO: make infos temporary -> destroy on resource initialization
	
	struct Pipeline::Layout::Impl{
		std::shared_ptr<Internal::Graphics::Pipelines::PipelineLayout> layout;
		Internal::Graphics::Pipelines::PipelineLayoutConfigInfo info;
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
		std::shared_ptr<Internal::Graphics::Pipelines::GraphicsPipeline> pipeline;
		Internal::Graphics::Pipelines::GraphicsPipelineConfigInfo info;
		Context* context;

		Impl(Context& context) : context{&context}{}
	};
}

#endif