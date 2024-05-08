#include <Raindrop/Asset.hpp>
#include <Raindrop/Context.hpp>
#include <Raindrop_internal/Context.hpp>
#include <Raindrop_internal/Assets/Manager.hpp>

#define ASSET_MANAGER context.getInternalContext()->getAssetManager()

namespace Raindrop{
	void Asset::Loader::collectGarbage(){}

	void Asset::UnregisterLoader(Context& context, const std::string& assetType){
		ASSET_MANAGER.unregisterLoader(assetType);
	}

	void Asset::_RegisterLoader(Context& context, const std::string& assetType, const std::shared_ptr<Loader>& loader){
		ASSET_MANAGER.registerLoader(assetType, loader);
	}

	std::shared_ptr<Asset> Asset::_Load(Context& context, const std::string& assetType, const Path& path){
		return ASSET_MANAGER.load(assetType, path);
	}
}