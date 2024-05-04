#ifndef __RAINDROP_CORE_ASSETS_MANAGER_INL_HPP__
#define __RAINDROP_CORE_ASSETS_MANAGER_INL_HPP__

#include "Manager.hpp"

namespace Raindrop::Core::Assets{
	template<typename T>
	inline std::shared_ptr<T> Manager::getAsset(const std::string& type, const Path& path){
		return std::static_pointer_cast<T>(getAsset(type, path));
	}

	template<typename T, typename... Args>
	void Manager::createLoader(const std::string& type, Args&&... args){
		static_assert(std::is_base_of_v<Loader, T>);
		
		std::shared_ptr<T> loader;
		try{
			loader = std::make_shared<T>(args...);
		} catch (const std::exception& e){
			throw std::runtime_error("Failed to create loader");
		}

		registerLoader(type, std::dynamic_pointer_cast<Loader>(loader));
	}
}

#endif