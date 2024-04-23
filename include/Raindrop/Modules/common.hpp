#ifndef __RAINDROP_MODULES_COMMON_HPP__
#define __RAINDROP_MODULES_COMMON_HPP__

#include "../common.hpp"
#include <Raindrop/Graphics/RenderSystems/common.hpp>

namespace Raindrop::Modules{
	using RenderSystem = Graphics::RenderSystems::RenderSystem;
	using RenderSystemID = Graphics::RenderSystems::RenderSystemID;
	using RenderSystemCollection = Graphics::RenderSystems::RenderSystemCollection;

	class Module;
	class ModuleInterface;
}

#endif