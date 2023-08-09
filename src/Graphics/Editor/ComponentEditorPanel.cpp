#include <Raindrop/Graphics/Editor/ComponentEditorPanel.hpp>
#include <Raindrop/Graphics/Editor/EditorContext.hpp>

#include <Raindrop/Core/Scene/Components/Camera.hpp>
#include <Raindrop/Core/Scene/Components/Hierarchy.hpp>
#include <Raindrop/Graphics/Components/Model.hpp>
#include <Raindrop/Graphics/Components/Sun.hpp>
#include <Raindrop/Graphics/Components/Spotlight.hpp>
#include <Raindrop/Graphics/Components/LightPoint.hpp>
#include <Raindrop/Core/Scene/Components/Tag.hpp>
#include <Raindrop/Core/Scene/Components/Transform.hpp>

namespace Raindrop::Graphics::Editor{
	ComponentEditorPanel::ComponentEditorPanel(EditorContext& context) : _context{context}{}
	ComponentEditorPanel::~ComponentEditorPanel(){}

	#define draw_component(T) if (entity.hasComponent<Raindrop::Core::Scene::Components::T>()){entity.getComponent<Raindrop::Core::Scene::Components::T>().UI(_context.context.context);}

	#define draw_graphics_component(T) if (entity.hasComponent<Raindrop::Graphics::Components::T>()){entity.getComponent<Raindrop::Graphics::Components::T>().UI(_context.context.context);}
	
	#define add_component(T) if (!entity.hasComponent<Raindrop::Core::Scene::Components::T>()) {if (ImGui::MenuItem(#T)){entity.createComponent<Raindrop::Core::Scene::Components::T>();}}

	#define add_graphics_component(T) if (!entity.hasComponent<Raindrop::Graphics::Components::T>()) {if (ImGui::MenuItem(#T)){entity.createComponent<Raindrop::Graphics::Components::T>();}}

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

				draw_graphics_component(Model);
				draw_graphics_component(LightPoint);
				draw_graphics_component(Spotlight);
				draw_graphics_component(Sun);

				if (ImGui::BeginPopup("add component")){
					add_component(Transform);
					add_component(Camera);

					add_graphics_component(Model);
					add_graphics_component(LightPoint);
					add_graphics_component(Spotlight);
					add_graphics_component(Sun);
					
					ImGui::EndPopup();
				}
			}

		}
		ImGui::End();
	}
}