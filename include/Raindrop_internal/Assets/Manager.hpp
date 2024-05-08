#ifndef __RAINDROP_INTERNAL_ASSET_MANAGER_HPP__
#define __RAINDROP_INTERNAL_ASSET_MANAGER_HPP__

#include "common.hpp"

namespace Raindrop::Internal::Assets{
	class Manager{
		public:
			Manager(Context& context);
			~Manager();

			void registerLoader(const std::string& assetType, const std::shared_ptr<Loader>& loader);
			void unregisterLoader(const std::string& assetType);

			std::shared_ptr<Asset> load(const std::string& assetType, const Path& path);

		private:
			Context& _context;

			struct AssetType{
				std::shared_ptr<Loader> loader;
				std::unordered_map<Path, std::weak_ptr<Asset>> pathToAssets;
			};

			std::unordered_map<std::string, AssetType> _typeToLoader;
	};
}

#endif