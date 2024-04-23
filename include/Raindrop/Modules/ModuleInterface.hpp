#ifndef __RAINDROP_MODULES_MODULE_INTERFACE_HPP__
#define __RAINDROP_MODULES_MODULE_INTERFACE_HPP__

#include "common.hpp"

namespace Raindrop::Modules{
	class ModuleInterface{
		public:
			ModuleInterface() = default;
			virtual ~ModuleInterface() = default;

			virtual const std::vector<RenderSystemID>& renderSystemIDs() const;
			virtual const std::vector<RenderSystemCollection>& renderSystemCollections() const;

	};
}

#endif