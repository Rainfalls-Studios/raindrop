#ifndef __RAINDROP_ASSETS_MANAGER_HPP__
#define __RAINDROP_ASSETS_MANAGER_HPP__

#include "types.hpp"
#include "pch.pch"

namespace Raindrop::Assets{
	class Manager{
		public:
			Manager() noexcept;
			~Manager();

			Manager& prepare(Context& context);
			void initialize();
			void release();

			void insertFactory(const std::size_t& typeID, std::unique_ptr<Factory>&& factory);

			template<typename T, typename... Args>
			void emplaceFactory(Args&&... args){
				static_assert(std::is_base_of_v<Factory, T>, "The factory MUST be a derived class of Raindrop::Assets::Factory");
				std::unique_ptr<T> factory = std::make_unique<T>(std::forward<Args>(args)...);
				insertFactory(typeid(T).hash_code(), std::move(factory));
			}

			void removeFactory(const std::size_t& typeID);

			template<typename T>
			void removeFactory(){
				removeFactory(typeid(T).hash_code());
			}

			std::shared_ptr<Asset> loadOrGet(const std::size_t& typeID, const std::filesystem::path& path);

			template<typename T>
			std::shared_ptr<T> loadOrGet(const std::filesystem::path& path){
				static_assert(std::is_base_of_v<Asset, T>, "The factory MUST be a derived class of Raindrop::Assets::Factory");
				return std::dynamic_pointer_cast<T>(loadOrGet(typeid(T).hash_code(), path));
			}

		private:
			Context* _context;
			std::unordered_map<std::size_t, std::unique_ptr<Factory>> _factories;
	};
}

#endif