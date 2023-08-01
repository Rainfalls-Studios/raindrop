#include <Raindrop/Graphics/Editor/SceneHierarchyPanel.hpp>
#include <Raindrop/Graphics/Editor/EditorContext.hpp>
#include <Raindrop/Core/Scene/Entity.hpp>
#include <Raindrop/Graphics/Editor/Icon.hpp>

#include <Raindrop/Graphics/Components/Model.hpp>
#include <Raindrop/Core/Scene/Components/Camera.hpp>
#include <Raindrop/Core/Scene/Components/Spotlight.hpp>
#include <Raindrop/Core/Scene/Components/LightPoint.hpp>
#include <Raindrop/Core/Scene/Components/Sun.hpp>

namespace Raindrop::Graphics::Editor{
	SceneHierarchyPanel::SceneHierarchyPanel(EditorContext& context) : _context{context}{}
	SceneHierarchyPanel::~SceneHierarchyPanel(){}

	void SceneHierarchyPanel::update(){
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		bool begin = ImGui::Begin("Scene Hierarchy", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus);
		ImGui::PopStyleVar();
		
		if (begin){
			_dockspace = ImGui::GetID("DockSpace");
			ImGui::DockSpace(_dockspace, ImVec2(0, 0), ImGuiDockNodeFlags_AutoHideTabBar);

			drawScene(_context.scene);
		}

		ImGui::End();
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
			bool hasChilds = !entity.hierarchy().childs.empty();

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
		
		if (_renamingEntity == entity){
			if (ImGui::InputText("##name", _renameBuffer, sizeof(_renameBuffer), ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_EnterReturnsTrue)){
				entity.tag().name = _renameBuffer;
				_renamingEntity = Core::Scene::Entity();
			}

			if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) && !ImGui::IsItemClicked()){
				_renamingEntity = Core::Scene::Entity();
			}
			
		} else {
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

			if (hovered && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)){
				renameEntity(entity);
			}

			// Icon, text
			ImGui::SameLine();
			ImGui::SetCursorPosX(x);
			float size = ImGui::GetFrameHeight();
			ImGui::Image(objectIcon.texture(), ImVec2(size, size), objectIcon.uv1(), objectIcon.uv2());
			ImGui::SameLine();
			

			if (opened){
				ImGui::Text(name.c_str());
			} else {
				ImGui::TextDisabled(name.c_str());
			}
		}

		ImGui::EndGroup();

		dragDrop(entity);

		if (ImGui::BeginPopupContextItem("Settings")){
			entitySettings(entity);
			ImGui::EndPopup();
		}

		if (*opened){
			ImGui::TreePush(name.c_str());
			auto it = entity.begin();
			while (it != entity.end()){
				auto child = *it;
				++it;
				drawEntity(child);
			}
			ImGui::TreePop();
		}


		ImGui::PopID();
	}

	void SceneHierarchyPanel::drawScene(Core::Scene::Scene* scene){
		if (!scene) return;

		ImGui::SetNextWindowDockID(_dockspace);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

		bool begin = ImGui::Begin("Scene", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus);

		ImGui::PopStyleVar();

		if (begin){
			drawEntity(Core::Scene::Entity(scene->root(), scene));
			
			ImGui::Dummy(ImGui::GetContentRegionAvail());

			if (ImGui::IsItemClicked(ImGuiMouseButton_Left)){
				_context.selectedEntity = Core::Scene::Entity(Core::Scene::INVALID_ENTITY_ID, nullptr);
			}
			
			if (ImGui::BeginPopupContextItem("Scene selection")){
				drawSceneSettings(scene);
				ImGui::EndPopup();
			}
		}

		ImGui::End();
	}

	void SceneHierarchyPanel::entitySettings(Core::Scene::Entity entity){
		if (ImGui::MenuItem("Select")) selectEntity(entity);
		if (ImGui::MenuItem("Add Component")) addComponentEntity(entity);
		if (ImGui::MenuItem("Add Child")) addChildEntity(entity);

		ImGui::Separator();
		if (ImGui::MenuItem("Look at")) lookAtEntity(entity);

		ImGui::Separator();
		
		if (ImGui::MenuItem("Copy", "Ctr+C")) copyEntity(entity);
		if (ImGui::MenuItem("Cut", "Ctr+X")) cutEntity(entity);
		if (ImGui::MenuItem("Copy ID", "Ctr+Maj+C")) copyEntityID(entity);

		ImGui::Separator();
		if (ImGui::MenuItem("Rename", "F2")) renameEntity(entity);
		if (ImGui::MenuItem("Remove", "Del", nullptr, entity != _context.scene->root())) removeEntity(entity);
	}

	void SceneHierarchyPanel::drawSceneSettings(Core::Scene::Scene* scene){
		if (ImGui::BeginMenu("Add")){
			if (ImGui::MenuItem("Cube", nullptr, nullptr, false)) addCube(scene);
			if (ImGui::MenuItem("Cone", nullptr, nullptr, false)) addCone(scene);
			if (ImGui::MenuItem("Plane", nullptr, nullptr, false)) addPlane(scene);
			if (ImGui::MenuItem("Pyramide", nullptr, nullptr, false)) addPyramide(scene);
			if (ImGui::MenuItem("Model")) addModel(scene);

			ImGui::Separator();
			if (ImGui::MenuItem("Text", nullptr, nullptr, false)) addText(scene);
			if (ImGui::MenuItem("3D Text", nullptr, nullptr, false)) add3DText(scene);

			ImGui::Separator();
			if (ImGui::MenuItem("Camera")) addCamera(scene);

			ImGui::Separator();
			if (ImGui::MenuItem("Light Point")) addLightPoint(scene);
			if (ImGui::MenuItem("Spotlight")) addSpotLight(scene);
			if (ImGui::MenuItem("Sun")) addSun(scene);

			ImGui::EndMenu();
		}
	}

	void SceneHierarchyPanel::dragDrop(Core::Scene::Entity entity){
		if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID)){
			ImGui::SetDragDropPayload("ENTITY", &entity, sizeof(Core::Scene::Entity));
			ImGui::Text(entity.tag().name.c_str());
			ImGui::EndDragDropSource();
		}

		if (ImGui::BeginDragDropTarget()){
			const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ENTITY");
			if (payload){
				Core::Scene::Entity child;
				memcpy(&child, payload->Data, sizeof(child));
			}
			ImGui::EndDragDropTarget();
		}
	}

	void SceneHierarchyPanel::selectEntity(Core::Scene::Entity entity){
		_context.selectedEntity = entity;
	}

	void SceneHierarchyPanel::addComponentEntity(Core::Scene::Entity entity){
		//TODO: Add 'Add component entity" - https://trello.com/c/6TYvsAXe/9-add-add-component-entity
	}

	void SceneHierarchyPanel::addChildEntity(Core::Scene::Entity entity){
		auto child = entity.createChild();
		selectEntity(child);
		renameEntity(child);
	}

	void SceneHierarchyPanel::lookAtEntity(Core::Scene::Entity entity){
		//TODO: Add "look at entity" - https://trello.com/c/Co2RWK5L/8-add-look-at-entity
	}

	void SceneHierarchyPanel::copyEntity(Core::Scene::Entity entity){
		//TODO: Add "copy entity" - https://trello.com/c/jNXXxisz/11-add-copy-entity
	}

	void SceneHierarchyPanel::cutEntity(Core::Scene::Entity entity){
		copyEntity(entity);
		removeEntity(entity);
	}

	void SceneHierarchyPanel::copyEntityID(Core::Scene::Entity entity){
		ImGui::SetClipboardText(std::to_string(entity.tag().UUID).c_str());
	}

	void SceneHierarchyPanel::renameEntity(Core::Scene::Entity entity){
		_renamingEntity = entity;
		strcpy(_renameBuffer, entity.tag().name.c_str());
	}

	void SceneHierarchyPanel::removeEntity(Core::Scene::Entity entity){
		if (entity == _renamingEntity) _renamingEntity = Core::Scene::Entity();
		if (entity == _context.selectedEntity) _context.selectedEntity = Core::Scene::Entity();
		
		try{
			entity.scene()->destroyEntity(entity.id());
		} catch (const std::exception &){}
	}

	Core::Scene::Entity SceneHierarchyPanel::createEntity(Core::Scene::Scene* scene){
		auto entity = Core::Scene::Entity(scene->root(), scene).createChild();
		selectEntity(entity);
		return entity;
	}

	void SceneHierarchyPanel::addCube(Core::Scene::Scene* scene){
		auto entity = createEntity(scene);
	}

	void SceneHierarchyPanel::addCone(Core::Scene::Scene* scene){
		auto entity = createEntity(scene);

	}

	void SceneHierarchyPanel::addPlane(Core::Scene::Scene* scene){
		auto entity = createEntity(scene);
	}

	void SceneHierarchyPanel::addPyramide(Core::Scene::Scene* scene){
		auto entity = createEntity(scene);

	}

	void SceneHierarchyPanel::addModel(Core::Scene::Scene* scene){
		auto entity = createEntity(scene);
		entity.createComponent<Components::Model>();
	}

	void SceneHierarchyPanel::addText(Core::Scene::Scene* scene){
		auto entity = createEntity(scene);

	}

	void SceneHierarchyPanel::add3DText(Core::Scene::Scene* scene){
		auto entity = createEntity(scene);

	}

	void SceneHierarchyPanel::addCamera(Core::Scene::Scene* scene){
		auto entity = createEntity(scene);
		entity.createComponent<Core::Scene::Components::Camera>();
	}
	
	void SceneHierarchyPanel::addLightPoint(Core::Scene::Scene* scene){
		auto entity = createEntity(scene);
		entity.createComponent<Core::Scene::Components::LightPoint>();
	}

	void SceneHierarchyPanel::addSpotLight(Core::Scene::Scene* scene){
		auto entity = createEntity(scene);
		entity.createComponent<Core::Scene::Components::Spotlight>();
	}

	void SceneHierarchyPanel::addSun(Core::Scene::Scene* scene){
		auto entity = createEntity(scene);
		entity.createComponent<Core::Scene::Components::Sun>();
	}
}