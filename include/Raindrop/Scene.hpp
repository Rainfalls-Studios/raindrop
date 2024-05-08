#ifndef __RAINDROP_SCENE_HPP__
#define __RAINDROP_SCENE_HPP__

#include "common.hpp"
#include <entt/entt.hpp>
#include <Raindrop/Asset.hpp>

namespace Raindrop{
	class Scene : public entt::registry, Asset{};
}

#endif