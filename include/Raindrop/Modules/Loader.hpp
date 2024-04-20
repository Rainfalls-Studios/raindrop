#ifndef __RAINDROP_MODULES_LOADER_HPP__
#define __RAINDROP_MODULES_LOADER_HPP__

#include "common.hpp"
#include <Raindrop/Core/Assets/Loader.hpp>

namespace Raindrop::Modules{
	class Loader : public Core::Assets::Loader{
		public:
			using Asset = Core::Assets::Asset;

			Loader(Context &context);
			virtual ~Loader() override;

			virtual void collectGarbage() override;
			virtual std::shared_ptr<Asset> load(const Path& path) override;
			virtual void registerAsset(const Path& path, const std::shared_ptr<Asset>& asset) override;

			std::shared_ptr<Module> get(const std::string& alias);
			
		private:
			Context& _context;
			std::unordered_map<std::string, std::weak_ptr<Module>> _aliasToModule;
			std::list<std::shared_ptr<Module>> _modules;

			void loadAliases(const std::shared_ptr<Module>& module);
	};
}

#endif