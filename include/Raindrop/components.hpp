#ifndef __RAINDROP_COMPONENTS_HPP__
#define __RAINDROP_COMPONENTS_HPP__

#include <Raindrop/common.hpp>
#include <Raindrop/Core/Scene/Components/common.hpp>
#include <Raindrop/Graphics/Components/common.hpp>

namespace Raindrop::Components{
	using ::Raindrop::Core::Scene::Components::Transform;
	using ::Raindrop::Core::Scene::Components::Tag;
	using ::Raindrop::Core::Scene::Components::Hierarchy;
	using ::Raindrop::Graphics::Components::Camera;
	using ::Raindrop::Graphics::Components::Sun;
	using ::Raindrop::Graphics::Components::LightPoint;
	using ::Raindrop::Graphics::Components::Spotlight;
	using ::Raindrop::Graphics::Components::Model;
}

#endif