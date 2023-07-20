#include <Raindrop/Graphics/Editor/SceneHierarchyPanel.hpp>
#include <Raindrop/Graphics/Editor/EditorContext.hpp>
#include <Raindrop/Core/Scene/Entity.hpp>
#include <Raindrop/Graphics/Editor/Icon.hpp>

namespace Raindrop::Graphics::Editor{
	SceneHierarchyPanel::SceneHierarchyPanel(EditorContext& context) : _context{context}{}
	SceneHierarchyPanel::~SceneHierarchyPanel(){}

	void SceneHierarchyPanel::update(){
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

		bool begin = _open ? ImGui::Begin("Scene Hierarchy", &_open, ImGuiWindowFlags_NoDocking) : false;
		
		ImGui::PopStyleVar(3);
		
		if (begin){
			_dockspace = ImGui::GetID("DockSpace");
			ImGui::DockSpace(_dockspace, ImVec2(0, 0), ImGuiDockNodeFlags_PassthruCentralNode);

			drawScene(_context.scene);

			ImGui::End();
		}
	}

	void SceneHierarchyPanel::drawEntity(Core::Scene::Entity entity){
		Icon& objectIcon = _context.icons.get("Cube");

		ImGui::PushID(entity.id());

		const ImGuiStyle& style = ImGui::GetStyle();
		ImGuiStorage* storage = ImGui::GetStateStorage();
		std::string name = entity.tag().name;

		bool* opened = storage->GetBoolRef(ImGui::GetID("opened"), false);

		ImGui::BeginGroup();

		// the arrow button
		{
			bool hasChilds = !entity.hierachy().childs.empty();

			if (hasChilds){
				if (ImGui::ArrowButton("arrow button", *opened ? ImGuiDir_Down : ImGuiDir_Right)){
					*opened = !*opened;
				}
			} else {
				float size = ImGui::GetFrameHeight();
				ImGui::Dummy(ImVec2(size, size));
			}
			ImGui::SameLine();
		}

		float x = ImGui::GetCursorPosX();
		if (ImGui::InvisibleButton("select button", ImVec2(-1, ImGui::GetFontSize() * 1.2))){
			_context.selectedEntity = entity;
		}

		bool hovered = ImGui::IsItemHovered();
		bool active = ImGui::IsItemActive();

		if (hovered || active){
			ImGui::GetWindowDrawList()->AddRectFilled(ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), ImGui::GetColorU32(active ? style.Colors[ImGuiCol_HeaderActive] : style.Colors[ImGuiCol_HeaderHovered]));
		} else if (_context.selectedEntity == entity){
			ImGui::GetWindowDrawList()->AddRectFilled(ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), ImGui::GetColorU32(style.Colors[ImGuiCol_TextSelectedBg]));
		}

		// Icon, text
		ImGui::SameLine();
		ImGui::SetCursorPosX(x + 5);
		float size = ImGui::GetFrameHeight();
		ImGui::Image(objectIcon.texture(), ImVec2(size, size), objectIcon.uv1(), objectIcon.uv2());
		ImGui::SameLine();

		if (opened){
			ImGui::Text(name.c_str());
		} else {
			ImGui::TextDisabled(name.c_str());
		}

		ImGui::EndGroup();

		if (*opened){
			ImGui::TreePush(name.c_str());
			for (const auto& child : entity){
				drawEntity(child);
			}
			ImGui::TreePop();
		}

		if (ImGui::BeginPopupContextItem("Settings")){
			entitySettings(entity);
			ImGui::EndPopup();
		}

		ImGui::PopID();
	}

	void SceneHierarchyPanel::drawScene(Core::Scene::Scene* scene){
		if (!scene) return;
		ImGui::SetNextWindowDockID(_dockspace);
		if (ImGui::Begin("Scene", nullptr, ImGuiWindowFlags_NoMove)){
			drawEntity(Core::Scene::Entity(scene->root(), scene));

			
			ImGui::Dummy(ImGui::GetContentRegionAvail());
			if (ImGui::IsItemClicked(ImGuiMouseButton_Right)){
				ImGui::OpenPopup("Scene selection");
			}
			
			if (ImGui::BeginPopup("Scene selection")){
				drawSceneSettings(scene);
				ImGui::EndPopup();
			}

			ImGui::End();
		}
	}

	void SceneHierarchyPanel::entitySettings(Core::Scene::Entity entity){
		ImGui::MenuItem("Select");
		ImGui::MenuItem("Add Component");

		ImGui::Separator();
		ImGui::MenuItem("Look at");

		ImGui::Separator();
		ImGui::MenuItem("Copy", "Ctr+C");
		ImGui::MenuItem("Cut", "Ctr+X");
		ImGui::MenuItem("Copy ID", "Ctr+Maj+C");

		ImGui::Separator();
		ImGui::MenuItem("Rename", "F2");
		ImGui::MenuItem("Remove", "Del");
	}

	void SceneHierarchyPanel::drawSceneSettings(Core::Scene::Scene* scene){
		ImGui::MenuItem("Hide");

		ImGui::MenuItem("Advenced settings");
	}
}