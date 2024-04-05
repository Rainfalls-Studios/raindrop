#include <Raindrop/Graphics/Models/Loader.hpp>
#include <Raindrop/Graphics/Models/Model.hpp>
#include <Raindrop/Graphics/Context.hpp>

#include <spdlog/spdlog.h>

namespace Raindrop::Graphics::Models{
	Loader::Loader(Context &context) : _context{context}{
		spdlog::info("Constructing model asset loader ...");
	}

	Loader::~Loader(){
		spdlog::info("Destroying model asset loader ... ({} models)", _models.size());
		_models.clear();
	}

	std::shared_ptr<Loader::Asset> Loader::load(const Path& path){
		spdlog::info("Loading model from \"{}\"", path.string());

		std::shared_ptr<Model> model;
		try{
			model = std::make_shared<Model>(_context, path);
		} catch (const std::exception& e){
			spdlog::error("Failed to load model from \"{}\" :: reason : \"{}\"", path.string(), e.what());
			throw std::runtime_error("Failed to load model");
		}
		_models.push_back(model);

		return std::static_pointer_cast<Loader::Asset>(model);
	}

	void Loader::registerAsset(const Path& path, const std::shared_ptr<Asset>& asset){
		_models.push_back(std::static_pointer_cast<Model>(asset));
	}

	void Loader::collectGarbage(){
		auto it = _models.begin();
		std::size_t count = 0;
		while (it != _models.end()){
			if (it->unique()){
				count++;
				_models.erase(it);
				continue;
			}
			it++;
		}

		spdlog::trace("Cleared {} model assets from model loader garbage collection", count);
	}
}