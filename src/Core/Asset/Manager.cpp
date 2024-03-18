#include <Raindrop/Core/Asset/Manager.hpp>
#include <Raindrop/Core/Asset/Loader.hpp>
#include <cassert>

#include <spdlog/spdlog.h>

namespace Raindrop::Core::Asset{
	Manager::Manager(Context& context) : _context{context}{
		spdlog::info("Creating asset manager...");
	}
	
	Manager::~Manager(){
		spdlog::info("Destructing asset manager...");
	}

	std::weak_ptr<Asset> Manager::get(const std::string& type, const Path& path){
		spdlog::info("Requirering asset \"{}\"... (type : \"{}\")", path.string(), type);

		TypeData* typeData = getTypeData(type);
		if (typeData == nullptr){
			spdlog::warn("Failed to find a loader corresponding to \"{}\" asset type for asset \"{}\"", type, path.string());
			throw std::runtime_error("Failed to find corresponding loader");
		}

		auto& pathToAsset = typeData->_pathToAssets;

		{
			auto it = pathToAsset.find(type);
			if (it != pathToAsset.end()){
				spdlog::trace("Found asset \"{}\" (type : \"{}\")", path.string(), type);
				return it->second;
			}
		}

		auto loader = typeData->_loader;
		assert(loader != nullptr && "The asset type is not linked to any asset loader");

		spdlog::info("Loading asset \"{}\"... (type : \"{}\")", path.string(), type);
		std::shared_ptr<Asset> asset = loader->load(path);
		pathToAsset.emplace(path, asset);

		return asset;
	}

	void Manager::registerLoader(const std::string& type, const std::shared_ptr<Loader>& loader){
		spdlog::trace("Registering asset loader for \"{}\" asset type", type);
		assert(findLoader(type) == nullptr && "The asset type is already linked with a loader");

		std::unique_ptr<TypeData> data = std::make_unique<TypeData>();
		data->_loader = loader;

		_typeToLoader.emplace(type, std::move(data));
	}

	Manager::TypeData* Manager::getTypeData(const std::string& type) const noexcept{
		auto it = _typeToLoader.find(type);
		if (it == _typeToLoader.end()) return nullptr;
		return it->second.get();
	}

	std::shared_ptr<Loader> Manager::findLoader(const std::string& type) const noexcept{
		TypeData* typeData = getTypeData(type);
		return typeData ? typeData->_loader : nullptr;
	}

	void Manager::unregisterType(const std::string& type){
		_typeToLoader.erase(type);
	}
}