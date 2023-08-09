#include <Raindrop/Graphics/Components/LightPoint.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Raindrop::Graphics::Components{
	LightPoint::LightPoint(){
		color = glm::vec3(1.f);
		intensity = 1.f;
	}

	void LightPoint::UI(Core::EngineContext& context){
		ImGui::PushID(this);
		
		if (ImGui::TreeNode("Light point")){
			ImGui::DragFloat("intensity", &intensity);
			ImGui::ColorPicker3("color", glm::value_ptr(color));

			ImGui::TreePop();
		}
		ImGui::PopID();
	}
}