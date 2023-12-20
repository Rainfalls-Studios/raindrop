#include <Raindrop/Graphics/Framebuffer/FramebufferBuilder.hpp>
#include <Raindrop/Graphics/Framebuffer/Context.hpp>
#include <Raindrop/Core/Engine.hpp>

namespace Raindrop::Graphics::Framebuffer{
	FramebufferBuilder::FramebufferBuilder(Context& context) : _context{context}{}
	FramebufferBuilder::~FramebufferBuilder(){}

	void FramebufferBuilder::loadFromNode(const YAML::Node& node){
		YAML::Mark mark = node.Mark();

		_context.logger().info("Loading new framebuffer at line {}", mark.line);

		_name = node["name"].as<std::string>();
		YAML::decodeVkFramebufferCreateFlags(node["flags"], _flags);
		_renderPass = _context.graphics().renderPass().renderPassManager().get(node["renderPass"].as<std::string>());

		for (const auto& attachment : node["attachments"]){
			loadAttachment(attachment);
		}

		auto& registry = _context.graphics().core().registry();

		_width = registry.get<uint32_t>(node["width"].as<std::string>());
		_height = registry.get<uint32_t>(node["height"].as<std::string>());
		_layers = registry.get<uint32_t>(node["layers"].as<std::string>("1"));
	}

	void FramebufferBuilder::loadAttachment(const YAML::Node& node){
		YAML::Mark mark = node.Mark();

		std::shared_ptr<Image::ImageView> attachment;
		try{
			attachment = _context.graphics().image().imageViewManager().get(node.as<std::string>());
		} catch (const std::exception &e){
			_context.logger().warn("Failed to find attachment for framebuffer at line {}", mark.line);
			return;
		}

		_attachments.push_back(attachment);
	}

	const std::string& FramebufferBuilder::name() const{
		return _name;
	}

	VkFramebufferCreateFlags FramebufferBuilder::flags() const{
		return _flags;
	}

	const std::shared_ptr<RenderPass::RenderPass>& FramebufferBuilder::renderPass() const{
		return _renderPass;
	}

	const std::vector<std::shared_ptr<Image::ImageView>>& FramebufferBuilder::attachments() const{
		return _attachments;
	}

	uint32_t FramebufferBuilder::width() const{
		return _width;
	}

	uint32_t FramebufferBuilder::height() const{
		return _height;
	}

	uint32_t FramebufferBuilder::layers() const{
		return _layers;
	}
}