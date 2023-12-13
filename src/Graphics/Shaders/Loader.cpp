#include <Raindrop/Graphics/Shaders/Loader.hpp>
#include <Raindrop/Graphics/Shaders/Context.hpp>
#include <Raindrop/Graphics/Shaders/GraphicsPipeline.hpp>
#include <Raindrop/Graphics/Shaders/GraphicsPipelineBuilder.hpp>
#include <Raindrop/Graphics/Shaders/GraphicsPipelineManager.hpp>
#include <Raindrop/Graphics/Shaders/PipelineLayout.hpp>
#include <Raindrop/Graphics/Shaders/PipelineLayoutBuilder.hpp>

namespace Raindrop::Graphics::Shaders{
	Loader::Loader(Context& context) : _context{context}{}
	Loader::~Loader(){}

	void Loader::loadPipelineLayouts(const YAML::Node& node){
		_context.logger().info("Loading pipeline layout...");
		if (!node.IsSequence()){
			_context.logger().error("Invalid node type ! excepting block sequence"),
			throw std::runtime_error("Invalid node type");
		}

		for (const auto& layoutNode : node){
			auto mark = layoutNode.Mark();

			_context.logger().trace("Loading \"{}\" pipeline layout at line {}", layoutNode.Tag(), mark.line);

			PipelineLayoutBuilder builder(_context);
			builder.loadFromNode(layoutNode);

			try{
				_context.pipelineLayoutManager().create(builder);
			} catch (const std::exception &e){
				_context.logger().error("Failed to load pipeline layout at line {} : {}", mark.line, e.what());
				throw std::runtime_error("Failed to load pipeline layout pipeline");
			}
		}
	}

	void Loader::loadGraphicsPipelines(const YAML::Node& node){
		_context.logger().info("Loading graphics pipelines...");
		if (!node.IsSequence()){
			_context.logger().error("Invalid node type ! excepting block sequence"),
			throw std::runtime_error("Invalid node type");
		}

		for (const auto& pipelineNode : node){
			auto mark = pipelineNode.Mark();

			_context.logger().trace("Loading \"{}\" graphics pipeline at line {}", pipelineNode.Tag(), mark.line);

			GraphicsPipelineBuilder builder(_context);
			builder.loadFromNode(pipelineNode);

			try{
				_context.graphicsPipelineManager().create(builder);
			} catch (const std::exception &e){
				_context.logger().error("Failed to load graphics pipeline at line {} : {}", mark.line, e.what());
				throw std::runtime_error("Failed to load graphics pipeline");
			}
		}
	}
}