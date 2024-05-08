#ifndef __RAINDROP_INLINE_ASSET_INL__
#define __RAINDROP_INLINE_ASSET_INL__

#include "../Asset.hpp"

namespace Raindrop{
	template<typename T, typename... Args>
	std::shared_ptr<T> Asset::CreateLoader(Context& context, const std::string& assetType, Args&&... args){
		static_assert(std::is_base_of_v<Loader, T>);

		std::shared_ptr<T> loader = std::make_shared<T>(std::forward<Args>(args)...);
		_RegisterLoader(context, assetType, std::static_pointer_cast<Loader>(loader));
		return loader;
	}

	template<typename T>
	void Asset::RegisterLoader(Context& context, const std::string& assetType, const std::shared_ptr<T>& loader){
		_RegisterLoader(context, assetType, std::static_pointer_cast<Loader>(loader));
	}

	template<typename T>
	std::shared_ptr<T> Asset::Load(Context& context, const std::string& assetType, const Path& path){
		static_assert(std::is_base_of_v<Asset, T>);

		auto asset = _Load(context, assetType, path);
		return std::static_pointer_cast<T>(asset);
	}
}

#endif