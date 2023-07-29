#include <Raindrop/Graphics/Editor/ComponentEditorPanel.hpp>
#include <Raindrop/Graphics/Editor/EditorContext.hpp>

#include <Raindrop/Core/Scene/Components/Camera.hpp>
#include <Raindrop/Core/Scene/Components/Hierarchy.hpp>
#include <Raindrop/Core/Scene/Components/Model.hpp>
#include <Raindrop/Core/Scene/Components/Tag.hpp>
#include <Raindrop/Core/Scene/Components/Transform.hpp>
#include <Raindrop/Core/Scene/Components/Spotlight.hpp>
#include <Raindrop/Core/Scene/Components/LightPoint.hpp>
#include <Raindrop/Core/Scene/Components/Sun.hpp>

namespace Raindrop::Graphics::Editor{
	ComponentEditorPanel::ComponentEditorPanel(EditorContext& context) : _context{context}{}
	ComponentEditorPanel::~ComponentEditorPanel(){}

	#define draw_component(T) if (entity.hasComponent<Raindrop::Core::Scene::Components::T>()){entity.getComponent<Raindrop::Core::Scene::Components::T>().UI(_context.context.context);}
	
	#define add_component(T) if (!entity.hasComponent<Raindrop::Core::Scene::Components::T>()) {if (ImGui::MenuItem(#T)){entity.createComponent<Raindrop::Core::Scene::Components::T>();}}

	void ComponentEditorPanel::update(Core::Scene::Entity entity){
		if (ImGui::Begin("Components", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus)){

			if (entity.id() != Core::Scene::INVALID_ENTITY_ID){
				if (ImGui::Button("add component")){
					ImGui::OpenPopup("add component");
				}

				draw_component(Tag);
				draw_component(Transform);
				draw_component(Hierarchy);
				draw_component(Camera);
				draw_component(Model);
				draw_component(LightPoint);
				draw_component(Spotlight);
				draw_component(Sun);

				if (ImGui::BeginPopup("add component")){
					add_component(Transform);
					add_component(Camera);
					add_component(Model);
					add_component(LightPoint);
					add_component(Spotlight);
					add_component(Sun);
					ImGui::EndPopup();
				}
			}

		}
		ImGui::End();
	}
}