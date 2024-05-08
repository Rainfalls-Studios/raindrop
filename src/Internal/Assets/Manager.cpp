#include <Raindrop_internal/Assets/Manager.hpp>

namespace Raindrop::Internal::Assets{
	Manager::Manager(Context& context) : _context{context}{}
	Manager::~Manager(){
		_typeToLoader.clear();
	}

	void Manager::registerLoader(const std::string& assetType, const std::shared_ptr<Loader>& loader){
		_typeToLoader[assetType].loader = loader;
	}

	void Manager::unregisterLoader(const std::string& assetType){
		_typeToLoader.erase(assetType);
	}

	std::shared_ptr<Asset> Manager::load(const std::string& assetType, const Path& path){
		AssetType& type = _typeToLoader.at(assetType);
		
		auto it = type.pathToAssets.find(path);
		if (it != type.pathToAssets.end()){
			auto asset = it->second;

			if (!asset.expired()){
				return asset.lock();
			}
		}

		std::shared_ptr<Asset> asset = type.loader->load(path);
		type.pathToAssets[path] = asset;
		return asset;
	}
}