#ifndef __RAINDROP_GRAPHICS_EDITOR_CONTEXT_HPP__
#define __RAINDROP_GRAPHICS_EDITOR_CONTEXT_HPP__

#include <Raindrop/Graphics/Editor/common.hpp>
#include <Raindrop/Graphics/Editor/ViewportFramebuffer.hpp>
#include <Raindrop/Graphics/Editor/FileExplorer.hpp>
#include <Raindrop/Graphics/GraphicsContext.hpp>

namespace Raindrop::Graphics::Editor{
	struct EditorContext{
		GraphicsContext& context;
		
		ViewportFramebuffer viewport;
		FileExplorer fileExplorer;

		EditorContext(GraphicsContext& context);
	};
}

#endif