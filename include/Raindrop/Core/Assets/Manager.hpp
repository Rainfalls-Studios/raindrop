#pragma once

#include "Raindrop/pch.pch"

namespace Raindrop::Core::Assets{
	class Asset;
	class Factory;
	class Context;

	class Manager{
		public:
			Manager(Context& context) noexcept;
			~Manager();

			void insertFactory(const std::type_info& typeInfo, std::shared_ptr<Factory>&& factory);

			template<typename Asset, typename T, typename... Args>
			std::shared_ptr<T> emplaceFactory(Args&&... args){
				static_assert(std::is_base_of_v<Factory, T>, "The factory MUST be a derived class of Raindrop::Core::Assets::Factory");
				std::shared_ptr<T> factory = std::make_shared<T>(std::forward<Args>(args)...);
				insertFactory(typeid(Asset), std::static_pointer_cast<Factory>(factory));
				return factory;
			}

			std::shared_ptr<Asset> loadOrGet(const std::type_info& typeInfo, const std::filesystem::path& path);

			template<typename T>
			std::shared_ptr<T> loadOrGet(const std::filesystem::path& path){
				static_assert(std::is_base_of_v<Asset, T>, "The factory MUST be a derived class of Raindrop::Core::Assets::Factory");
				return std::dynamic_pointer_cast<T>(loadOrGet(typeid(T), path));
			}

		private:
			Context& _context;
			std::unordered_map<std::size_t, std::weak_ptr<Factory>> _factories;
	};
}