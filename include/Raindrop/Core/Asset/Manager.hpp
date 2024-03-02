#ifndef __RAINDROP_CORE_ASSET_MANAGER_HPP__
#define __RAINDROP_CORE_ASSET_MANAGER_HPP__

#include "common.hpp"
#include "Asset.hpp"
#include "../Memory/GuardedPointer.hpp"

namespace Raindrop::Core::Asset{
	class Manager{
		public:
			Manager();
			~Manager();

			std::weak_ptr<Asset> get(const std::string& type, const Path& path);

			template<typename T>
			std::weak_ptr<T> get(const std::string& type, const Path& path){
				return std::dynamic_pointer_cast<T>(get(type, path));
			}
			
			void registerLoader(const std::string& type, const std::shared_ptr<Loader>& loader);

			template<typename T, typename... Args>
			void registerLoader(const std::string& type, Args&&... args){
				static_assert(std::is_base_of_v<Loader, T>);

				std::shared_ptr<T> loader = std::make_shared<T>(args...);
				registerLoader(type, std::dynamic_pointer_cast<Loader>(loader));
			}

			std::shared_ptr<Loader> findLoader(const std::string& type) const noexcept;

		private:
			std::unordered_map<Path, std::shared_ptr<Asset>> _pathToAsset;
			std::unordered_map<std::string, std::shared_ptr<Loader>> _typeToLoader;
	};
}

#endif