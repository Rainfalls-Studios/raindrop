#include <Raindrop_internal/Assets/Manager.hpp>
#include <Raindrop_internal/Context.hpp>

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
		AssetType* type = nullptr;
		try{
			type = &_typeToLoader.at(assetType);
		} catch (const std::out_of_range& e){
			_context.getLogger()->warn("Cannot find a loader coresponding to \"{}\" assets, cannot load \"{}\"", assetType, path.string());
		}
		
		auto it = type->pathToAssets.find(path);
		if (it != type->pathToAssets.end()){
			auto asset = it->second;

			if (!asset.expired()){
				return asset.lock();
			}
		}

		auto loader = type->loader.lock();
		if (!loader){
			_context.getLogger()->warn("The load coresponding to \"{}\" assets is expired, cannot load \"{}\"", assetType, path.string());
			return nullptr;
		}

		std::shared_ptr<Asset> asset = loader->load(path);
		type->pathToAssets[path] = asset;
		return asset;
	}
}