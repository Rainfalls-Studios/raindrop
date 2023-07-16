#ifndef __RAINDROP_GRAPHICS_EDITOR_WIDGETS_HPP__
#define __RAINDROP_GRAPHICS_EDITOR_WIDGETS_HPP__

#include <Raindrop/Graphics/Editor/common.hpp>

namespace Raindrop::Graphics::Editor{
	static inline ImVec2 operator+(const ImVec2& lhs, const ImVec2& rhs) { 
		return ImVec2(lhs.x + rhs.x, lhs.y + rhs.y); 
	}
	
	void ImageRotated(ImTextureID tex_id, ImVec2 size, float angle, ImVec2 uv0 = ImVec2(0, 0), ImVec2 uv1 = ImVec2(1, 1), ImVec2 center = ImVec2(123456789.f, 123456789.f));
}

#endif