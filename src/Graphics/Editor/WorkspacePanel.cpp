// #include <Raindrop/Graphics/Editor/WorkspacePanel.hpp>
// #include <Raindrop/Graphics/Editor/EditorContext.hpp>
// #include <Raindrop/Graphics/Editor/ViewportPanel.hpp>
// #include <Raindrop/Graphics/Editor/SceneEditorPanel.hpp>
// #include <imgui/imgui_internal.h>

// namespace Raindrop::Graphics::Editor{
// 	WorkspacePanel::WorkspacePanel(EditorContext& context) : _context{context}{
// 		_viewport = std::make_unique<ViewportPanel>(_context);
// 		_sceneEditor = std::make_unique<SceneEditorPanel>(_context);
// 	}

// 	WorkspacePanel::~WorkspacePanel(){}

// 	void WorkspacePanel::update(){
//         ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
//         ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
//         ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

// 		bool begin = ImGui::Begin("Workspace", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus);

// 		ImGui::PopStyleVar(3);

// 		if (begin){
// 			ImGuiID dockspaceID = ImGui::GetID("Dockpace");
// 			ImGui::DockSpace(dockspaceID, ImVec2(0, 0), ImGuiDockNodeFlags_NoDocking | ImGuiDockNodeFlags_NoTabBar);

// 			static bool firstTime = true;
// 			if (firstTime){
// 				firstTime = false;

// 				ImGui::DockBuilderRemoveNode(dockspaceID);
// 				ImGui::DockBuilderAddNode(dockspaceID);
// 				ImGui::DockBuilderSetNodeSize(dockspaceID, ImGui::GetMainViewport()->Size);

// 				auto dock_id_left = ImGui::DockBuilderSplitNode(dockspaceID, ImGuiDir_Left, 0.01, nullptr, &dockspaceID);
// 				auto dock_id_right = ImGui::DockBuilderSplitNode(dockspaceID, ImGuiDir_Right, 0.05f, nullptr, &dockspaceID);

// 				ImGui::DockBuilderDockWindow("Toolbar", dock_id_left);
// 				ImGui::DockBuilderDockWindow("Scene Editor", dock_id_right);
// 				ImGui::DockBuilderDockWindow("Viewport", dockspaceID);

// 				ImGui::DockBuilderFinish(dockspaceID);

// 			}
			
// 			_viewport->update();
// 			_sceneEditor->update();
// 			_context.toolbar.update();
// 		}

// 		ImGui::End();
// 	}
// }