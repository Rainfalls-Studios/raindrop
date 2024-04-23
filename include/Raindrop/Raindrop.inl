#ifndef __RAINDROP_RAINDROP_INL__
#define __RAINDROP_RAINDROP_INL__

#include "Raindrop.hpp"

namespace Raindrop{
	
	// === Events ===

	template<typename F>
	inline constexpr void Raindrop::subscribeEvent(const std::string& name, F&& callback){
		eventManager().subscribe(name, callback);
	}

	template<typename... Args>
	inline constexpr void Raindrop::triggerEvent(const std::string& name, Args... args){
		eventManager().trigger<Args...>(name, args...);
	}

	template<typename... Args>
	inline constexpr void Raindrop::registerEvent(const std::string& name){
		eventManager().registerEvent<Args...>(name);
	}

	// === Assets ===

	template<typename T>
	inline constexpr std::shared_ptr<T> Raindrop::getAsset(const std::string& type, const Path& path){
		static_assert(std::is_base_of_v<Asset, T> && "The asset type as to be based of Raindrop::Asset (Raindrop::Core::Assets::Asset)");
		return std::static_pointer_cast<T>(getAsset(type, path));
	}

	template<typename T, typename... Args>
	inline constexpr void Raindrop::createAssetLoader(const std::string& type, Args&&... args){
		static_assert(std::is_base_of_v<AssetLoader, T> && "The asset loader has to be based of Raindrop::AssetLoader (Raindrop::Core::Assets::Loader)");
		registerAssetLoader(type, std::make_shared<T>(args...));
	}

	template<typename T>
	inline constexpr std::shared_ptr<T> Raindrop::getAssetLoader(const std::string& type){
		return std::static_pointer_cast<T>(getAssetLoader(type));
	}

	// === Modules ===
	
	template<typename F>
	inline std::shared_ptr<F> Raindrop::getModule(const std::string& alias){
		return std::static_pointer_cast<F>(getModule(alias));
	}

	template<typename F>
	inline std::shared_ptr<F> Raindrop::loadModule(const Path& path){
		return std::static_pointer_cast<F>(loadModule(path));
	}

	// === Graphics ===
}

#endif