#include <Raindrop/Renderer/Model/Loader.hpp>
#include <Raindrop/Renderer/Model/Model.hpp>
#include <Raindrop/Renderer/Context.hpp>

#include <spdlog/spdlog.h>

namespace Raindrop::Renderer::Model{
	Loader::Loader(Context &context) : _context{context}{
		spdlog::info("Constructing model asset loader ...");
	}

	Loader::~Loader(){
		spdlog::info("Destructing model asset loader ...");
	}

	std::shared_ptr<::Raindrop::Core::Asset::Asset> Loader::load(const Path& path){
		spdlog::info("Loading model from \"{}\"", path.string());

		std::shared_ptr<Model> model;
		try{
			model = std::make_shared<Model>(_context, path);
		} catch (const std::exception& e){
			spdlog::error("Failed to load model from \"{}\" :: reason : \"{}\"", path.string(), e.what());
			throw std::runtime_error("Failed to load model");
		}
		_models.push_back(model);

		return std::static_pointer_cast<Asset>(model);
	}

	void Loader::registerAsset(const Path& path, const std::shared_ptr<Asset>& asset){
		std::shared_ptr<Model> model = std::static_pointer_cast<Model>(asset);
		_models.push_back(model);
	}
}