#ifndef __RAINDROP_GRAPHICS_COMMON_HPP__
#define __RAINDROP_GRAPHICS_COMMON_HPP__

#include <vulkan/vulkan.h>
#include <vulkan/vk_enum_string_helper.h>
#include <SDL2/SDL.h>

#include <Raindrop/common.hpp>
#include <Raindrop/Core/tools.hpp>
#include <Raindrop/Core/Scene/Scene.hpp>
#include <Raindrop/Core/Context.hpp>
#include <Raindrop/Core/Asset/AssetFactory.hpp>
#include <Raindrop/Core/Asset/Asset.hpp>

namespace Raindrop::Graphics{
	struct Context;
	class Engine;
	class FrameState;
	class Camera;
}

#endif