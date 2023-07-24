#include <Raindrop/Graphics/Editor/ViewportPanel.hpp>
#include <Raindrop/Graphics/Editor/EditorContext.hpp>
#include <imgui/imgui_internal.h>
#include <glm/gtc/constants.hpp>
#include <ImGuizmo/ImGuizmo.h>
#include <glm/gtc/type_ptr.hpp>

namespace Raindrop::Graphics::Editor{
	ViewportPanel::ViewportPanel(EditorContext& context) : _context{context}{}
	ViewportPanel::~ViewportPanel(){}

	void ViewportPanel::update(){
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

		bool begin = ImGui::Begin("Viewport", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);

		ImGui::PopStyleVar(3);

		if (begin){
			ImVec2 pos = ImGui::GetCursorScreenPos();
			ImVec2 winPos = ImGui::GetWindowPos();
			ImVec2 winSize = ImGui::GetWindowSize();

			ImVec2 origin = ImGui::GetCursorPos();

			_start = {pos.x, pos.y};
			_size = {winSize.x, winSize.y};

			ImGui::GetWindowDrawList()->AddImage(
				(void *)_context.viewport.texture(),
				ImVec2(_start.x, _start.y),
				ImVec2(_size.x + _start.x, _size.y + _start.y),
				ImVec2(1, 1), ImVec2(0, 0)
			);

			ImVec2 size = ImGui::GetWindowSize();
			auto& camera = _context.camera;
			camera.width = glm::vec2(-size.x/2.f, size.x/2.f);
			camera.height = glm::vec2(-size.y/2.f, size.y/2.f);

			ImGui::SetCursorPos(origin);

			if (ImGui::IsWindowFocused()){
				ImVec2 translationDrag = ImGui::GetMouseDragDelta(ImGuiMouseButton_Middle);
				ImVec2 rotationDrag = ImGui::GetMouseDragDelta(ImGuiMouseButton_Right);

				ImGui::ResetMouseDragDelta(ImGuiMouseButton_Middle);
				ImGui::ResetMouseDragDelta(ImGuiMouseButton_Right);

				auto& rotation = camera.rotation;

				const float sensitivity = 0.1f;
				rotation += glm::vec3(rotationDrag.y * sensitivity, -rotationDrag.x * sensitivity, 0.0f);
				rotation.x = glm::clamp(rotation.x, -89.0f, 89.0f);

				ImGui::Text("x : %f", camera.rotation.x);
				ImGui::Text("y : %f", camera.rotation.y);
				ImGui::Text("z : %f", camera.rotation.z);

				if (ImGui::IsMouseDown(ImGuiMouseButton_Right)){
					auto& io = ImGui::GetIO();
					translationDrag.y -= io.MouseWheel;
				}

				glm::vec3 translation = camera.translationSensivity * (camera.right * -translationDrag.x + camera.forward * -translationDrag.y);

				camera.translation += translation;
			}

		}

		guizmo();
		ImGui::End();
	}

	
	glm::u32vec2 ViewportPanel::start() const{
		return _start;
	}

	glm::u32vec2 ViewportPanel::size() const{
		return _size;
	}

	void ViewportPanel::guizmo(){
		if (_context.selectedEntity.id() == Core::Scene::INVALID_ENTITY_ID) return;
		if (!_context.selectedEntity.hasComponent<Core::Scene::Components::Transform>()) return;

		ImGuizmo::SetImGuiContext(ImGui::GetCurrentContext());
		ImGuizmo::SetOrthographic(_context.camera.type == Camera::Type::ORTHOGRAPHIC);
		ImGuizmo::SetDrawlist(ImGui::GetWindowDrawList());

		ImGuizmo::SetRect(_start.x, _start.y, _size.x, _size.y);

		const glm::mat4 projection = glm::scale(_context.camera.projection, glm::vec3(1.f, -1.f, 1.f));
		const glm::mat4& view =_context.camera.view;

		auto& transform = _context.selectedEntity.getComponent<Core::Scene::Components::Transform>();
		glm::mat4 transformMatrix;

		ImGuizmo::RecomposeMatrixFromComponents(
			glm::value_ptr(transform.translation),
			glm::value_ptr(transform.rotation),
			glm::value_ptr(transform.scale),
			glm::value_ptr(transformMatrix)
		);
		
		ImGuizmo::Manipulate(
			glm::value_ptr(view),
			glm::value_ptr(projection),
			ImGuizmo::TRANSLATE,
			ImGuizmo::LOCAL,
			glm::value_ptr(transformMatrix)
		);

		if (ImGuizmo::IsUsing()){
			ImGuizmo::DecomposeMatrixToComponents(
				glm::value_ptr(transformMatrix),
				glm::value_ptr(transform.translation),
				glm::value_ptr(transform.rotation),
				glm::value_ptr(transform.scale)
			);
		}
	}
}