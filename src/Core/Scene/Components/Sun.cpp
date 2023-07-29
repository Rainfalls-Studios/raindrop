#include <Raindrop/Core/Scene/Components/Sun.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Raindrop::Core::Scene::Components{
	Sun::Sun(){
		color = glm::vec3(1.f);
	}

	void Sun::UI(EngineContext& context){
		ImGui::PushID(this);
		
		if (ImGui::TreeNode("Sun")){
			ImGui::ColorPicker3("color", glm::value_ptr(color));

			ImGui::TreePop();
		}
		ImGui::PopID();
	}
}