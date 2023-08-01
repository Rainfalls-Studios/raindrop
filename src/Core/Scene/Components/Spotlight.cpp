#include <Raindrop/Core/Scene/Components/Spotlight.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Raindrop::Core::Scene::Components{
	Spotlight::Spotlight(){
		color = glm::vec3(1.f);
		intensity = 1.f;
		cutOff = 1.f;
		outerCutOff = 1.3f;
	}

	void Spotlight::UI(EngineContext& context){
		ImGui::PushID(this);
		
		if (ImGui::TreeNode("Spotlight")){
			ImGui::DragFloat("intensity", &intensity);

			float degCutoff = glm::degrees(cutOff);
			if (ImGui::DragFloat("cutOff", &degCutoff)){
				cutOff = glm::radians(degCutoff);
			}

			float degOuterCutOff = glm::degrees(outerCutOff);
			if (ImGui::DragFloat("outer cutOff", &outerCutOff)){
				outerCutOff = glm::radians(degOuterCutOff);
			}

			ImGui::ColorPicker3("color", glm::value_ptr(color));

			ImGui::TreePop();
		}
		ImGui::PopID();
	}
}