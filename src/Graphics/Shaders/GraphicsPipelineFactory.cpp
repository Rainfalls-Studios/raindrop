#include <Raindrop/Graphics/Shaders/GraphicsPipelineFactory.hpp>
#include <Raindrop/Graphics/Shaders/GraphicsPipeline.hpp>
#include <Raindrop/Graphics/Shaders/Context.hpp>

// #include <Raindrop/Graphics/Shaders/GraphicsPipelineLoaders/YamlLoader.hpp>

namespace Raindrop::Graphics::Shaders{

	GraphicsPipelineFactory::GraphicsPipelineFactory(Context& context) : _context{context}{}
	GraphicsPipelineFactory::~GraphicsPipelineFactory(){}

	std::shared_ptr<Core::Asset::Asset> GraphicsPipelineFactory::createAsset(const std::filesystem::path& path){
		std::shared_ptr<GraphicsPipeline> pipeline;
		std::string extension = path.extension().string();

		if (extension == ".ymlgfxpipe"){
			// GraphicsPipelineLoaders::YamlLoader loader(_context, path);
			// pipeline = std::make_shared<GraphicsPipeline>()
		} else {
			throw std::runtime_error("Invalid file extension");
		}

		throw std::runtime_error("temp throw");
	}

	void GraphicsPipelineFactory::destroyAsset(std::shared_ptr<Core::Asset::Asset> asset){
		_pipelines.remove(std::static_pointer_cast<GraphicsPipeline>(asset));
	}

	std::vector<const char*> GraphicsPipelineFactory::extensions() const{
		return {
			".ymlgfxpipe"
		};
	}

	bool GraphicsPipelineFactory::has(const std::shared_ptr<Core::Asset::Asset>& asset) const{
		return std::find(_pipelines.begin(), _pipelines.end(), std::static_pointer_cast<GraphicsPipeline>(asset)) != _pipelines.end();
	}
}