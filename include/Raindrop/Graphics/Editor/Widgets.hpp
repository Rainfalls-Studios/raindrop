#ifndef __RAINDROP_GRAPHICS_EDITOR_WIDGETS_HPP__
#define __RAINDROP_GRAPHICS_EDITOR_WIDGETS_HPP__

#include <Raindrop/Graphics/Editor/common.hpp>

namespace ImGui{
	bool BufferingBar(const char* label, float value,  const ImVec2& size_arg, const ImU32& bg_col, const ImU32& fg_col);
	bool Spinner(const char* label, float radius, int thickness, const ImU32& color);
	void LoadingIndicatorCircle(const char* label, const float indicator_radius, const ImVec4& main_color, const ImVec4& backdrop_color, const int circle_count, const float speed);
}

#endif