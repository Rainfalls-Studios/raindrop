#ifndef __RAINDROP_GRAPHICS_EDITOR_CONTEXT_HPP__
#define __RAINDROP_GRAPHICS_EDITOR_CONTEXT_HPP__

#include <Raindrop/Graphics/Editor/common.hpp>
#include <Raindrop/Graphics/Editor/ViewportFramebuffer.hpp>
#include <Raindrop/Graphics/Editor/FileExplorer.hpp>
#include <Raindrop/Graphics/Editor/IconAtlas.hpp>
#include <Raindrop/Graphics/Editor/Fonts.hpp>
#include <Raindrop/Graphics/Editor/Camera.hpp>
#include <Raindrop/Graphics/GraphicsContext.hpp>
#include <Raindrop/Core/Scene/Entity.hpp>

namespace Raindrop::Graphics::Editor{
	static inline constexpr ImVec4 rgb(float r, float g, float b, float a = 255.f){
		return ImVec4(r/255.f, g/255.f, b/255.f, a/255.f);
	}

	struct EditorContext{
		GraphicsContext& context;
		
		ViewportFramebuffer viewport;
		FileExplorer fileExplorer;
		IconAtlas icons;
		Fonts fonts;
		
		Core::Scene::Entity selectedEntity;
		Core::Scene::Scene* scene;

		Camera camera;

		EditorContext(GraphicsContext& context);
	};
}

#endif