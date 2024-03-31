#ifndef __RAINDROP_CORE_ASSETS_MANAGER_HPP__
#define __RAINDROP_CORE_ASSETS_MANAGER_HPP__

#include "common.hpp"
#include "Asset.hpp"

namespace Raindrop::Core::Assets{
	class Manager{
		public:
			Manager(Context &context);
			~Manager();

			std::weak_ptr<Asset> get(const std::string& type, const Path& path);

			template<typename T>
			std::weak_ptr<T> get(const std::string& type, const Path& path){
				return std::dynamic_pointer_cast<T>(get(type, path).lock());
			}

			std::weak_ptr<Asset> find(const std::string& type, const Path& path);

			template<typename T>
			std::weak_ptr<T> find(const std::string& type, const Path& path){
				return std::dynamic_pointer_cast<T>(find(type, path).lock());
			}

			void registerAsset(const std::string& type, const Path& path, const std::shared_ptr<Asset>& asset);
			
			void registerLoader(const std::string& type, const std::shared_ptr<Loader>& loader);

			template<typename T, typename... Args>
			void registerLoader(const std::string& type, Args&&... args){
				static_assert(std::is_base_of_v<Loader, T>);
				
				std::shared_ptr<T> loader;
				try{
					loader = std::make_shared<T>(args...);
				} catch (const std::exception& e){
					throw std::runtime_error("Failed to create loader");
				}

				registerLoader(type, std::dynamic_pointer_cast<Loader>(loader));
			}

			std::shared_ptr<Loader> findLoader(const std::string& type) const noexcept;

			void unregisterType(const std::string& type);

		private:
			Context& _context;
			
			struct TypeData{
				std::shared_ptr<Loader> _loader;
				std::unordered_map<Path, std::shared_ptr<Asset>> _pathToAssets;
			};

			std::unordered_map<std::string, std::unique_ptr<TypeData>> _typeToLoader;
			TypeData* getTypeData(const std::string& type) const noexcept;
	};
}

#endif