#ifndef __RAINDROP_COMMON_HPP__
#define __RAINDROP_COMMON_HPP__

#include <Raindrop/pch.hpp>

#ifdef RAINDROP_EDITOR
	#define RAINDROP_EDITOR_DIR std::filesystem::path(RAINDROP_EDITOR_PATH)
#else
	#define RAINDROP_EDITOR_DIR std::filesystem::path("")
#endif

namespace Raindrop{
	class Raindrop;
	class Scene;
	class Entity;
}

#endif