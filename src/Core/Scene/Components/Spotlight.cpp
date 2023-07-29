#include <Raindrop/Core/Scene/Components/Spotlight.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Raindrop::Core::Scene::Components{
	Spotlight::Spotlight(){
		color = glm::vec3(1.f);
		intensity = 1.f;
		maxAngle = 1.f;
	}

	void Spotlight::UI(EngineContext& context){
		ImGui::PushID(this);
		
		if (ImGui::TreeNode("Spotlight")){
			ImGui::DragFloat("intensity", &intensity);
			ImGui::DragFloat("max angle", &maxAngle);
			ImGui::ColorPicker3("color", glm::value_ptr(color));

			ImGui::TreePop();
		}
		ImGui::PopID();
	}
}