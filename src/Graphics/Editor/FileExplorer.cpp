#include <Raindrop/Graphics/Editor/FileExplorer.hpp>
#include <Raindrop/Graphics/Editor/EditorContext.hpp>
#include <imgui/imgui_internal.h>

namespace Raindrop::Graphics::Editor{
	FileExplorer::FileExplorer(EditorContext& context): _context{context}{

	}

	FileExplorer::~FileExplorer(){

	}

	void FileExplorer::update(){

        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

		bool begin = ImGui::Begin("FileExplorer");

		ImGui::PopStyleVar(3);

		if (begin){
			ImGuiID dockspace_id = ImGui::GetID("Dockspace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_NoTabBar | ImGuiDockNodeFlags_NoDocking);

			static auto first_time = true;
			if (first_time) {
				first_time = false;
				ImGui::DockBuilderRemoveNode(dockspace_id);
				ImGui::DockBuilderAddNode(dockspace_id);
				ImGui::DockBuilderSetNodeSize(dockspace_id, ImGui::GetWindowSize());

				auto dock_id_left = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Left, 0.23f, nullptr, &dockspace_id);
				auto dock_id_right = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Right, 0.76f, nullptr, &dockspace_id);
				ImGui::DockBuilderDockWindow("Hierachy", dock_id_left);
				ImGui::DockBuilderDockWindow("Content", dock_id_right);

				ImGui::DockBuilderFinish(dockspace_id);
			}

			hierachy();
			content();
		}
		ImGui::End();
	}

	void FileExplorer::hierachy(){
		ImGui::PushStyleColor(ImGuiCol_WindowBg, rgb(197, 197, 197));

		if (ImGui::Begin("Hierachy")){
			
		}
		ImGui::End();

		ImGui::PopStyleColor();
	}

	void FileExplorer::content(){
		ImGui::PushStyleColor(ImGuiCol_WindowBg, rgb(217, 217, 217));

		if (ImGui::Begin("Content")){
			ImGui::Text("aaaa");
		}
		ImGui::End();

		ImGui::PopStyleColor();
	}
}