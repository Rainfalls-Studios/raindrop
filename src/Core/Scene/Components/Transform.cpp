#include <Raindrop/Core/Scene/Components/Transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Raindrop::Core::Scene::Components{
	void Transform::UI(EngineContext& context){
		if (ImGui::TreeNode("Translation")){
			ImGui::DragFloat3("translation", reinterpret_cast<float*>(&translation));
			ImGui::DragFloat3("scale", reinterpret_cast<float*>(&scale), 0.01);

			float r = glm::length(glm::vec3(rotation.x, rotation.y, rotation.z));

			glm::vec3 euler = glm::degrees(glm::eulerAngles(rotation));

			if (ImGui::DragFloat3("rotation", glm::value_ptr(euler), 0.01f)){
				rotation = glm::quat(glm::radians(euler));
			}

			ImGui::TreePop();
		}
	}
}