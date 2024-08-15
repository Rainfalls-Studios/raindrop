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

			void insertFactory(const std::type_info& typeInfo, std::shared_ptr<Factory>&& factory);

			template<typename Asset, typename T, typename... Args>
			std::shared_ptr<T> emplaceFactory(Args&&... args){
				static_assert(std::is_base_of_v<Factory, T>, "The factory MUST be a derived class of Raindrop::Assets::Factory");
				std::shared_ptr<T> factory = std::make_shared<T>(std::forward<Args>(args)...);
				insertFactory(typeid(Asset), std::static_pointer_cast<Factory>(factory));
				return factory;
			}

			std::shared_ptr<Asset> loadOrGet(const std::type_info& typeInfo, const std::filesystem::path& path);

			template<typename T>
			std::shared_ptr<T> loadOrGet(const std::filesystem::path& path){
				static_assert(std::is_base_of_v<Asset, T>, "The factory MUST be a derived class of Raindrop::Assets::Factory");
				return std::dynamic_pointer_cast<T>(loadOrGet(typeid(T), path));
			}

		private:
			Context* _context;
			std::unordered_map<std::size_t, std::weak_ptr<Factory>> _factories;
	};
}

#endif