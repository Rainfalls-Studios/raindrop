#include <Raindrop/Graphics/Editor/ViewportPanel.hpp>
#include <Raindrop/Graphics/Editor/EditorContext.hpp>
#include <imgui/imgui_internal.h>

namespace Raindrop::Graphics::Editor{
	ViewportPanel::ViewportPanel(EditorContext& context) : _context{context}{

	}

	ViewportPanel::~ViewportPanel(){

	}

	void ViewportPanel::update(){
		// ImGui::SetNextWindowDockID(ImGui::GetID("DockSpace"));
		
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

		bool begin = ImGui::Begin("Viewport", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);

		ImGui::PopStyleVar(3);

		if (begin){
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

			ImVec2 pos = ImGui::GetCursorScreenPos();
			ImVec2 winPos = ImGui::GetWindowPos();
			ImVec2 winSize = ImGui::GetWindowSize();

			ImGui::GetWindowDrawList()->AddImage(
				(void *)_context.viewport.texture(),
				ImVec2(pos.x - 12, pos.y - 7),
				ImVec2(winPos.x + winSize.x + 5, winPos.y + winSize.y),
				ImVec2(0, 1), ImVec2(1, 0)
			);

			ImGui::PopStyleVar();

		}
		ImGui::End();


	}
}