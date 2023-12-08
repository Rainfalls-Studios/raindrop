#include <Raindrop/Graphics/RenderPass/Loader.hpp>
#include <Raindrop/Graphics/RenderPass/Context.hpp>
#include <Raindrop/Graphics/RenderPass/RenderPassBuilder.hpp>

namespace Raindrop::Graphics::RenderPass{
	Loader::Loader(Context& context) : _context{context}{}
	Loader::~Loader(){}

	void Loader::loadRenderPasses(const YAML::Node& node){
		_context.logger().info("Loading render pass node");

		if (!node.IsSequence()){
			_context.logger().error("Invalid node type ! Excepting block sequence"),
			throw std::runtime_error("Invalid node type");
		}

		for (const auto& renderPassNode : node){
			auto mark = renderPassNode.Mark();

			_context.logger().trace("Loading \"{}\" render pass at line {}", renderPassNode.Tag(), mark.line);

			RenderPassBuilder builder(_context);
			builder.loadFromNode(renderPassNode);

			try{
				_context.renderPassManager().create(builder);
			} catch (const std::exception &e){
				_context.logger().error("Failed to load render pass node at line {}", mark.line);
				throw std::runtime_error("Failed to load render pass node");
			}
		}
	}
}