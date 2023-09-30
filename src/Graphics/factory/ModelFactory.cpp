#include <Raindrop/Graphics/Factory/ModelFactory.hpp>
#include <Raindrop/Graphics/Model.hpp>
#include <Raindrop/Graphics/GraphicsContext.hpp>
#include <Raindrop/Core/Asset/AssetManager.hpp>

namespace Raindrop::Graphics::Factory{
	ModelFactory::ModelFactory(GraphicsContext& context) : _context{context}{
	}

	ModelFactory::~ModelFactory(){
		_models.clear();
	}

	std::shared_ptr<Core::Asset::Asset> ModelFactory::createAsset(const std::filesystem::path& path){
		Builders::ModelBuilder builder;
		builder.loadFile(path);
		std::shared_ptr<Model> model = builder.build(_context);

		_models.push_back(model);
		return std::static_pointer_cast<Core::Asset::Asset>(model);
	}

	void ModelFactory::destroyAsset(std::shared_ptr<Core::Asset::Asset> asset){
		_models.remove(std::static_pointer_cast<Model>(asset));
	}

	std::vector<const char*> ModelFactory::extensions() const{
		return {
			".obj"
		};
	}
}