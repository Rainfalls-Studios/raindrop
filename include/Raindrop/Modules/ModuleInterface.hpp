#ifndef __RAINDROP_MODULES_MODULE_INTERFACE_HPP__
#define __RAINDROP_MODULES_MODULE_INTERFACE_HPP__

#include "common.hpp"
#include <Raindrop/Graphics/RenderSystems/RenderSystemCollection.hpp>

namespace Raindrop::Modules{
	class ModuleInterface{
		public:
			ModuleInterface() = default;
			virtual ~ModuleInterface() = default;

			virtual const std::unordered_map<std::string, RenderSystemCollection>& renderSystemCollections() const;

	};
}

#endif