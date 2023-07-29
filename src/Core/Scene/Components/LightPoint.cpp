#include <Raindrop/Core/Scene/Components/LightPoint.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Raindrop::Core::Scene::Components{
	LightPoint::LightPoint(){
		color = glm::vec3(1.f);
		intensity = 1.f;
	}

	void LightPoint::UI(EngineContext& context){
		ImGui::PushID(this);
		
		if (ImGui::TreeNode("Light point")){
			ImGui::DragFloat("intensity", &intensity);
			ImGui::ColorPicker3("color", glm::value_ptr(color));

			ImGui::TreePop();
		}
		ImGui::PopID();
	}
}