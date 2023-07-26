#include <Raindrop/Graphics/Editor/Toolbar.hpp>
#include <ImGuizmo/ImGuizmo.h>

namespace Raindrop::Graphics::Editor{
	Toolbar::Toolbar(EditorContext& context) : _context{context}{}
	Toolbar::~Toolbar(){};

	void Toolbar::update(){
		ImGui::SetNextWindowSize(ImVec2(-1.f, 25));
		if (ImGui::Begin("Toolbar", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove)){
			
			modeButton();
			operationButton();

		}
		ImGui::End();
	}

	void Toolbar::modeButton(){
		if (ImGui::Button("Mode")){
			ImGui::OpenPopup("Mode popup");
		}

		if (ImGui::BeginPopup("Mode popup")){
			if (ImGui::RadioButton("Local", reinterpret_cast<int*>(&guizmoMode), ImGuizmo::LOCAL)) guizmoMode = ImGuizmo::LOCAL;
			if (ImGui::RadioButton("World", reinterpret_cast<int*>(&guizmoMode), ImGuizmo::WORLD)) guizmoMode = ImGuizmo::WORLD;
			ImGui::EndPopup();
		}
	}

	void Toolbar::operationButton(){
		if (ImGui::Button("Tr")) guizmoOperation = ImGuizmo::TRANSLATE;
		if (ImGui::Button("Ro")) guizmoOperation = ImGuizmo::ROTATE_X | ImGuizmo::ROTATE_Y | ImGuizmo::ROTATE_Z;
		if (ImGui::Button("Sc")) guizmoOperation = ImGuizmo::SCALE;
	}
}