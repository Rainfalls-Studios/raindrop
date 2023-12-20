#include <Raindrop/Graphics/Loader.hpp>
#include <Raindrop/Graphics/Context.hpp>

#include <fstream>

namespace Raindrop::Graphics{
	Loader::Loader(Context& context) : _context{context}{}
	Loader::~Loader(){}

	void Loader::loadFile(const std::filesystem::path& path){
		_context.logger().info("Loading \'{}\" file ...", path.string());

		YAML::Node root = YAML::LoadFile(path);

		try{
			loadNode(root["raindrop"]["graphics"]);
		} catch (const std::exception &e){
			_context.logger().error("Failed to load \"{}\" file : {}", path.string(), e.what());
			throw std::runtime_error("Failed to load file");
		}
	}

	#define NODE(name, fnc) {name, [&](const YAML::Node& node){fnc(node);}}

	void Loader::loadNode(const YAML::Node& node){
		using FncType = std::function<void(const YAML::Node&)>;
		const std::unordered_map<std::string, FncType> nameToFnc = {
			NODE("formats", _context.formats().loader().loadFormats),
			NODE("images", _context.image().loader().loadImages),
			NODE("imageViews", _context.image().loader().loadImageViews),
			NODE("renderPasses", _context.renderPass().loader().loadRenderPasses),
			NODE("pipelineLayouts", _context.shaders().loader().loadPipelineLayouts),
			NODE("graphicsPipelines", _context.shaders().loader().loadGraphicsPipelines),
			NODE("framebuffers", _context.framebuffer().loader().loadFramebuffers),
			NODE("vertexLayouts", _context.buffers().loader().loadVertexLayouts),
			NODE("import", loadNode)
		};
		
		for (const auto& child : node){
			for (auto& it : child){
				std::string tag = it.first.Scalar();

				auto fncIt = nameToFnc.find(tag);
				if (fncIt == nameToFnc.end()){
					_context.logger().warn("Cannot recognize \"{}\" node at line {}", tag, it.first.Mark().line);
					continue;
				}
				
				fncIt->second(it.second);
			}
		}
	}
}