#include <Raindrop/Graphics/Image/Loader.hpp>
#include <Raindrop/Graphics/Image/Context.hpp>

#include <Raindrop/Graphics/Image/ImageBuilder.hpp>
#include <Raindrop/Graphics/Image/Image.hpp>

#include <Raindrop/Graphics/Image/ImageViewBuilder.hpp>
#include <Raindrop/Graphics/Image/ImageView.hpp>

namespace Raindrop::Graphics::Image{
	Loader::Loader(Context& context) : _context{context}{}
	Loader::~Loader(){}

	void Loader::loadImages(const YAML::Node& node){
		_context.logger().info("Loading image node");
		if (!node.IsSequence()){
			_context.logger().error("Invalid node type ! excepting block sequence"),
			throw std::runtime_error("Invalid node type");
		}

		for (const auto& imageNode : node){
			auto mark = imageNode.Mark();

			_context.logger().trace("Loading \"{}\" image node at line {}", imageNode.Tag(), mark.line);

			ImageBuilder builder(_context);
			builder.loadFromNode(imageNode);

			try{
				_context.imageManager().create(builder);
			} catch (const std::exception &e){
				_context.logger().error("Failed to load image node at line {}", mark.line);
				throw std::runtime_error("Failed to load image node");
			}
		}
	}

	void Loader::loadImageViews(const YAML::Node& node){
		_context.logger().info("Loading image views...");
		if (!node.IsSequence()){
			_context.logger().error("Invalid node type ! excepting block sequence"),
			throw std::runtime_error("Invalid node type");
		}

		for (const auto& imageViewNode : node){
			auto mark = imageViewNode.Mark();

			_context.logger().trace("Loading \"{}\" image view at line {}", imageViewNode.Tag(), mark.line);

			ImageViewBuilder builder(_context);
			builder.loadFromNode(imageViewNode);

			try{
				_context.imageViewManager().create(builder);
			} catch (const std::exception &e){
				_context.logger().error("Failed to load image view node at line {}", mark.line);
				throw std::runtime_error("Failed to load image view node");
			}
		}
	}
}