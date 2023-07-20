#include <Raindrop/Graphics/Editor/ComponentEditorPanel.hpp>
#include <Raindrop/Graphics/Editor/EditorContext.hpp>

#include <Raindrop/Core/Scene/Components/Camera.hpp>
#include <Raindrop/Core/Scene/Components/Hierarchy.hpp>
#include <Raindrop/Core/Scene/Components/Model.hpp>
#include <Raindrop/Core/Scene/Components/Tag.hpp>
#include <Raindrop/Core/Scene/Components/Transform.hpp>

namespace Raindrop::Graphics::Editor{
	ComponentEditorPanel::ComponentEditorPanel(EditorContext& context) : _context{context}{}
	ComponentEditorPanel::~ComponentEditorPanel(){}

	#define draw_component(T) if (entity.hasComponent<Raindrop::Core::Scene::Components::T>()){entity.getComponent<Raindrop::Core::Scene::Components::T>().UI(_context.context.context);}
	
	#define add_component(T) if (!entity.hasComponent<Raindrop::Core::Scene::Components::T>()) {if (ImGui::MenuItem(#T)){entity.createComponent<Raindrop::Core::Scene::Components::T>();}}

	void ComponentEditorPanel::update(Core::Scene::Entity entity){
		if (entity.id() == Core::Scene::INVALID_ENTITY_ID) return;

		if (ImGui::Begin("components", nullptr, ImGuiWindowFlags_NoDocking)){
			if (ImGui::Button("add component")){
				ImGui::OpenPopup("add component");
			}

			draw_component(Tag);
			draw_component(Transform);
			draw_component(Hierarchy);
			draw_component(Camera);
			draw_component(Model);

			if (ImGui::BeginPopup("add component")){
				add_component(Transform);
				add_component(Camera);
				add_component(Model);
				ImGui::EndPopup();
			}

			ImGui::End();
		}
	}
}