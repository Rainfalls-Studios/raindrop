#ifndef __RAINDROP_CORE_MODULES_MODULE_INTERFACE_HPP__
#define __RAINDROP_CORE_MODULES_MODULE_INTERFACE_HPP__

#include "common.hpp"

namespace Raindrop::Core::Modules{
	class ModuleInterface{
		public:
			ModuleInterface() = default;
			virtual ~ModuleInterface() = default;
	};
}

#endif