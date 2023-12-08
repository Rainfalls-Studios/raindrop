#include <Raindrop/Graphics/Framebuffer/Loader.hpp>
#include <Raindrop/Graphics/Framebuffer/Context.hpp>
#include <Raindrop/Graphics/Framebuffer/FramebufferBuilder.hpp>
#include <Raindrop/Graphics/Framebuffer/FramebufferManager.hpp>

namespace Raindrop::Graphics::Framebuffer{
	Loader::Loader(Context& context) : _context{context}{}
	Loader::~Loader(){}

	#define TRY(fnc, name) {const auto& child = node[name]; if (child) fnc(child);}

	void Loader::loadFramebuffers(const YAML::Node& node){
		_context.logger().info("Loading framebuffer node");
		if (!node.IsSequence()){
			_context.logger().error("Invalid node type ! excepting block sequence"),
			throw std::runtime_error("Invalid node type");
		}

		for (const auto& framebufferNode : node){
			auto mark = framebufferNode.Mark();

			_context.logger().trace("Loading \"{}\" framebuffer node at line {}", framebufferNode.Tag(), mark.line);

			FramebufferBuilder builder(_context);
			builder.loadFromNode(framebufferNode);

			try{
				_context.manager().create(builder);
			} catch (const std::exception &e){
				_context.logger().error("Failed to load framebuffer node at line {}", mark.line);
				throw std::runtime_error("Failed to load framebuffer node");
			}
		}
	}
}