#ifndef __RAINDROP_MODULES_MODULE_HPP__
#define __RAINDROP_MODULES_MODULE_HPP__

#include "common.hpp"
#include <Raindrop/Core/Assets/Asset.hpp>

namespace Raindrop::Modules{
	class Module : public Core::Assets::Asset{
		public:
			Module(Context& context, const Path& path);
			virtual ~Module();

			ModuleInterface* interface() const;

			const std::string& name() const;
			const std::string& description() const;
			const Version& version() const;
			const std::vector<std::string>& aliases() const;

			// ==== Render systems ====

			const std::vector<RenderSystemID>& renderSystemIDs() const;
			bool hasRenderSystem(const RenderSystemID& id);

			const std::vector<RenderSystemCollection>& renderSystemCollections() const;
			const RenderSystemCollection* getRenderSystemCollection(const std::string& name) const;

		private:
			struct __Module__;

			__Module__* _module;
			ModuleInterface* _interface;

	};
}

#endif