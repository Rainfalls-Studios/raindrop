#include <Raindrop/Graphics/Loader.hpp>
#include <Raindrop/Graphics/Context.hpp>

#include <fstream>

namespace Raindrop::Graphics{
	Loader::Loader(Context& context) : _context{context}{}
	Loader::~Loader(){}

	void Loader::loadFile(const std::filesystem::path& path){
		_context.logger().info("Loading {} file ...", path.string());

		YAML::Node root = YAML::LoadFile(path);

		try{
			loadNode(root["raindrop"]["graphics"]);
		} catch (const std::exception &e){
			_context.logger().error("Failed to load \"{}\" file : {}", path.string(), e.what());
			throw std::runtime_error("Failed to load file");
		}
	}

	#define TRY(fnc, name) {const auto& child = node[name]; if (child) fnc(child);}

	void Loader::loadNode(const YAML::Node& node){
		TRY(_context.formats().loader().loadFormats, "formats");
		TRY(_context.image().loader().loadImages, "images");
		TRY(_context.image().loader().loadImageViews, "imageViews");
		TRY(_context.renderPass().loader().loadRenderPasses, "renderPasses");
		TRY(_context.framebuffer().loader().loadFramebuffers, "framebuffers");
	}
}