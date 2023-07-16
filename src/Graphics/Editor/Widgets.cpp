#include <Raindrop/Graphics/Editor/Widgets.hpp>
#include <imgui/imgui_internal.h>

namespace Raindrop::Graphics::Editor{
	void ImageRotated(ImTextureID tex_id, ImVec2 size, float angle, ImVec2 uv0, ImVec2 uv1, ImVec2 center){
		if (center.x == 123456789.f){
			center.x = size.x / 2.f;
		}

		if (center.y == 123456789.f){
			center.y = size.y / 2.f;
		}

		ImDrawList* draw_list = ImGui::GetWindowDrawList();

		float cos_a = cosf(angle);
		float sin_a = sinf(angle);

		ImVec2 pos[4] = {
			center + ImRotate(ImVec2(-size.x * 0.5f, -size.y * 0.5f), cos_a, sin_a),
			center + ImRotate(ImVec2(+size.x * 0.5f, -size.y * 0.5f), cos_a, sin_a),
			center + ImRotate(ImVec2(+size.x * 0.5f, +size.y * 0.5f), cos_a, sin_a),
			center + ImRotate(ImVec2(-size.x * 0.5f, +size.y * 0.5f), cos_a, sin_a)
		};

		ImVec2 uvs[4] = { 
			ImVec2(uv0.x, uv0.y), 
			ImVec2(uv1.x, uv0.y), 
			ImVec2(uv1.x, uv1.y), 
			ImVec2(uv0.x, uv1.y)
		};

		draw_list->AddImageQuad(tex_id, pos[0], pos[1], pos[2], pos[3], uvs[0], uvs[1], uvs[2], uvs[3], IM_COL32_WHITE);
	}
}