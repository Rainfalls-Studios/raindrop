// #include <Raindrop/Graphics/Editor/ViewportPanel.hpp>
// #include <Raindrop/Graphics/Editor/EditorContext.hpp>
// #include <imgui/imgui_internal.h>
// #include <glm/gtc/constants.hpp>
// #include <ImGuizmo/ImGuizmo.h>
// #include <glm/gtc/type_ptr.hpp>
// #include <glm/gtx/matrix_decompose.hpp>
// // #include <Raindrop/Core/Scene/Components/Model.hpp>
// #include <Raindrop/Graphics/Model.hpp>

// namespace Raindrop::Graphics::Editor{
// 	ViewportPanel::ViewportPanel(EditorContext& context) : _context{context}{}
// 	ViewportPanel::~ViewportPanel(){}

// 	void ViewportPanel::update(){
// 		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
// 		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
// 		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

// 		bool begin = ImGui::Begin("Viewport", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);

// 		ImGui::PopStyleVar(3);

// 		if (begin){
// 			ImVec2 pos = ImGui::GetCursorScreenPos();
// 			ImVec2 winPos = ImGui::GetWindowPos();
// 			ImVec2 size = ImGui::GetWindowSize();

// 			ImVec2 origin = ImGui::GetCursorPos();

// 			_start = {pos.x, pos.y};
// 			_size = {size.x, size.y};

// 			ImGui::GetWindowDrawList()->AddImage(
// 				(void *)_context.viewport.texture(),
// 				ImVec2(_start.x, _start.y),
// 				ImVec2(_size.x + _start.x, _size.y + _start.y)
// 			);

// 			auto& camera = _context.camera;
// 			camera.width = glm::vec2(-size.x/2.f, size.x/2.f);
// 			camera.height = glm::vec2(-size.y/2.f, size.y/2.f);

// 			ImGui::SetCursorPos(origin);

// 			ImGui::Dummy(size);

// 			if (ImGui::IsWindowHovered){
// 				if (ImGui::IsMouseDown(ImGuiMouseButton_Right) || ImGui::IsMouseDown(ImGuiMouseButton_Middle)){
// 					ImGui::FocusWindow(ImGui::GetCurrentWindow(), ImGuiFocusRequestFlags_UnlessBelowModal);
// 				}
// 			}

// 			if (ImGui::IsWindowFocused()){
// 				ImVec2 translationDrag = ImGui::GetMouseDragDelta(ImGuiMouseButton_Middle, 0.f);
// 				ImVec2 rotationDrag = ImGui::GetMouseDragDelta(ImGuiMouseButton_Right, 0.f);

// 				ImGui::ResetMouseDragDelta(ImGuiMouseButton_Middle);
// 				ImGui::ResetMouseDragDelta(ImGuiMouseButton_Right);

// 				auto& rotation = camera.rotation;

// 				const float sensitivity = 0.1f;
// 				rotation += glm::vec3(rotationDrag.y * -sensitivity, -rotationDrag.x * sensitivity, 0.0f);
// 				rotation.x = glm::clamp(rotation.x, -89.0f, 89.0f);

// 				float forward = -ImGui::GetIO().MouseWheel;

// 				glm::vec3 translation = camera.translationSensivity * (camera.right * translationDrag.x + camera.up * translationDrag.y);
// 				translation += camera.forward * forward;

// 				camera.translation += translation;
// 			}

// 			// if (ImGui::BeginDragDropTarget()){
// 			// 	ImGui::GetWindowDrawList()->AddRectFilled(
// 			// 		ImVec2(_start.x, _start.y),
// 			// 		ImVec2(_start.x + _size.x, _start.y + _size.y),
// 			// 		ImGui::GetColorU32(ImGui::GetStyleColorVec4(ImGuiCol_ModalWindowDimBg))
// 			// 	);

// 			// 	std::filesystem::path path;
// 			// 	const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(MODEL_PATH_TYPE);
				
// 			// 	if (payload){
// 			// 		path = static_cast<char*>(payload->Data);

// 			// 		try{
// 			// 			auto entity = Core::Scene::Entity(_context.scene->createEntity(), _context.scene);
// 			// 			auto& component = entity.createComponent<Core::Scene::Components::Model>();
// 			// 			component._model = _context.context.context.assetManager.loadOrGet<Model>(path);
// 			// 		} catch (const std::exception& e){}
// 			// 	}
				
// 			// 	ImGui::EndDragDropTarget();
// 			// }
// 		}
		
// 		guizmo();
// 		ImGui::End();
// 	}
	
// 	glm::u32vec2 ViewportPanel::start() const{
// 		return _start;
// 	}

// 	glm::u32vec2 ViewportPanel::size() const{
// 		return _size;
// 	}

// 	void ViewportPanel::guizmo(){
// 		if (_context.selectedEntity.id() == Core::Scene::INVALID_ENTITY_ID) return;
// 		if (!_context.selectedEntity.hasComponent<Core::Scene::Components::Transform>()) return;

// 		ImGuizmo::SetImGuiContext(ImGui::GetCurrentContext());
// 		ImGuizmo::SetOrthographic(_context.camera.type == Camera::Type::ORTHOGRAPHIC);
// 		ImGuizmo::SetDrawlist(ImGui::GetWindowDrawList());

// 		ImGuizmo::SetRect(_start.x, _start.y, _size.x, _size.y);

// 		const glm::mat4 projection = glm::scale(_context.camera.projection, glm::vec3(1.f, -1.f, 1.f));
// 		const glm::mat4& view =_context.camera.view;

// 		auto& transform = _context.selectedEntity.getComponent<Core::Scene::Components::Transform>();

// 		glm::mat4 modelMatrix; // The complete model matrix (translation, rotation, scale)
// 		// Compose the complete model matrix from translation, rotation, and scale
// 		modelMatrix = glm::translate(glm::mat4(1.0f), transform.translation);
// 		modelMatrix = modelMatrix * glm::mat4_cast(transform.rotation);
// 		modelMatrix = glm::scale(modelMatrix, transform.scale);

// 		ImGuizmo::Manipulate(
// 			glm::value_ptr(view),
// 			glm::value_ptr(projection),
// 			ImGuizmo::OPERATION(_context.toolbar.guizmoOperation),
// 			ImGuizmo::MODE(_context.toolbar.guizmoMode),
// 			glm::value_ptr(modelMatrix)
// 		);

// 		if (ImGuizmo::IsUsing()){
// 			glm::vec3 skew;
// 			glm::vec4 perspective;

// 			glm::decompose(
// 				modelMatrix, 
// 				transform.scale,
// 				transform.rotation,
// 				transform.translation,
// 				skew,
// 				perspective
// 			);
// 		}
// 	}
// }