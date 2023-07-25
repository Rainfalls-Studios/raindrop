#include <Raindrop/Core/Scene/Components/Transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Raindrop::Core::Scene::Components{
	void Transform::UI(EngineContext& context){
		if (ImGui::TreeNode("Translation")){
			ImGui::DragFloat3("translation", reinterpret_cast<float*>(&translation));
			ImGui::DragFloat3("scale", reinterpret_cast<float*>(&scale), 0.01);

			float r = glm::length(glm::vec3(rotation.x, rotation.y, rotation.z));

			glm::vec3 rotationComponents;
			rotationComponents.x = (rotation.w + 1.f) * 90.f;
			rotationComponents.y = glm::acos(rotation.z);
			rotationComponents.z = glm::atan(rotation.y / rotation.x);
			
			ImGui::Text("x:%f, y:%f, z:%f, w:%f", rotation.x, rotation.y, rotation.z, rotation.w);
			if (ImGui::DragFloat3("rotation", glm::value_ptr(rotationComponents), 0.01f)){
				glm::vec3 vec;

				vec.x = glm::sin(rotationComponents.y) * glm::cos(rotationComponents.z);
				vec.y = glm::sin(rotationComponents.y) * glm::sin(rotationComponents.z);
				vec.z = glm::cos(rotationComponents.y);

				vec = glm::normalize(vec);
				rotation = glm::quat(rotation.w, vec);
			}

			ImGui::TreePop();
		}
	}
}