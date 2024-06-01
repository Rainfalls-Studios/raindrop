#ifndef __RAINDROP_INTERNAL_PIPELINE_HPP__
#define __RAINDROP_INTERNAL_PIPELINE_HPP__

#include <Raindrop/Pipeline.hpp>
#include <Raindrop/Context.hpp>
#include <Raindrop_internal/Graphics/GraphicsPipeline.hpp>
#include <Raindrop_internal/Graphics/PipelineLayout.hpp>
#include <Raindrop_internal/Graphics/Shader.hpp>
#include <Raindrop_internal/Color.hpp>

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
		// TODO: Should it shader ownership ?
		struct Info{
			struct Stage{
				std::shared_ptr<Shader> shader;
				std::string entryPoint;
				Shader::Stage stage;
				Shader::Flags flags;
			};

			std::vector<ColorAttachment> attachments;
			std::vector<std::pair<Vertex::Layout, Vertex::InputRate>> bindings;
			std::vector<Viewport> viewports;
			std::vector<Scissor> scissors;
			std::vector<DynamicState> dynamicStates;
			std::vector<Stage> stages;
			Internal::Graphics::GraphicsPipelineConfigInfo internal;
		};

		std::shared_ptr<Internal::Graphics::GraphicsPipeline> pipeline;
		Info info;
		Context* context;

		Impl(Context& context) : context{&context}{}
	};

	VkStencilOpState toVulkan(const Pipeline::StencilOpState& state);
	VkPipelineCreateFlags toVulkan(const Pipeline::Flags& flags);
	VkPipelineStageFlags toVulkan(const Pipeline::Stage& stage);
	VkPipelineColorBlendStateCreateFlags toVulkan(const Pipeline::ColorBlendFlags& flags);
	VkPipelineDepthStencilStateCreateFlags toVulkan(const Pipeline::DepthStencilFlags& flags);
	VkCullModeFlags toVulkan(const Pipeline::CullMode& mode);
	VkPrimitiveTopology toVulkan(const Pipeline::Topology& t);
	VkPolygonMode toVulkan(const Pipeline::PolygonMode& mode);
	VkFrontFace toVulkan(const Pipeline::FrontFace& f);
	VkSampleCountFlags toVulkan(const Pipeline::SampleCount& count);
	VkSampleCountFlagBits toVulkan(const Pipeline::SampleCount::Bits& count);
	VkStencilOp toVulkan(const Pipeline::StencilOperation& op);
	VkDynamicState toVulkan(const Pipeline::DynamicState& s);
	VkPipelineShaderStageCreateFlags toVulkan(const Pipeline::Shader::Flags& f);
	VkShaderStageFlagBits toVulkan(const Pipeline::Shader::Stage& s);
}

#endif