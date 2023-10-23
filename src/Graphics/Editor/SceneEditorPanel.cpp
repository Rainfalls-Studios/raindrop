#include <Raindrop/Graphics/Editor/SceneEditorPanel.hpp>
#include <Raindrop/Graphics/Editor/EditorContext.hpp>
#include <Raindrop/Graphics/Editor/SceneHierarchyPanel.hpp>
#include <Raindrop/Graphics/Editor/ComponentEditorPanel.hpp>
#include <imgui/imgui_internal.h>

namespace Raindrop::Graphics::Editor{
	SceneEditorPanel::SceneEditorPanel(EditorContext& context) : _context{context}{
		_sceneHierarchy = std::make_unique<SceneHierarchyPanel>(_context);
		_componentEditor = std::make_unique<ComponentEditorPanel>(_context);
	}

	SceneEditorPanel::~SceneEditorPanel(){

	}

	void SceneEditorPanel::update(){
		if (ImGui::Begin("Scene Editor", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus)){

			ImGuiID dockspaceID = ImGui::GetID("Dockpace");
			ImGui::DockSpace(dockspaceID, ImVec2(0, 0), ImGuiDockNodeFlags_NoDocking | ImGuiDockNodeFlags_NoTabBar);

			static bool firstTime = true;
			if (firstTime){
				firstTime = false;

				ImGui::DockBuilderRemoveNode(dockspaceID);
				ImGui::DockBuilderAddNode(dockspaceID);
				ImGui::DockBuilderSetNodeSize(dockspaceID, ImGui::GetMainViewport()->Size);

				auto dock_id_up = ImGui::DockBuilderSplitNode(dockspaceID, ImGuiDir_Up, 0.5, nullptr, &dockspaceID);
				auto dock_id_down = ImGui::DockBuilderSplitNode(dockspaceID, ImGuiDir_Down, 0.5, nullptr, &dockspaceID);

				ImGui::DockBuilderDockWindow("Scene Hierarchy", dock_id_up);
				ImGui::DockBuilderDockWindow("Components", dock_id_down);

				ImGui::DockBuilderFinish(dockspaceID);

			}
			_sceneHierarchy->update();
			_componentEditor->update(_context.selectedEntity);
		}
		

		ImGui::End();
	}

}