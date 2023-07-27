#include <Raindrop/Core/Scene/Components/Model.hpp>
#include <Raindrop/Graphics/Model.hpp>
#include <Raindrop/Graphics/GraphicsPipeline.hpp>
#include <Raindrop/Graphics/Texture.hpp>
#include <Raindrop/Graphics/Editor/DragDropHandler.hpp>

namespace Raindrop::Core::Scene::Components{
	void Model::UI(EngineContext& context){
		ImGui::PushID(this);

		ImGuiStorage* storage = ImGui::GetStateStorage();

		if (ImGui::TreeNode("Model")){
			modelUI(context);
			pipelineUI(context);
			textureUI(context);

			ImGui::TreePop();
		}
		ImGui::PopID();
	}

	void Model::modelUI(EngineContext& context){
		static char buff[255] = "";

		if (ImGui::InputText("model", buff, sizeof(buff), ImGuiInputTextFlags_EnterReturnsTrue)){
			try{
				_model = context.assetManager.loadOrGet<Graphics::Model>(buff);
			} catch (const std::exception& e){
				printf("%s\n", e.what());
			}
			buff[0] = '\0';
		}

		if (ImGui::BeginDragDropTarget()){
			std::filesystem::path path;
			const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(Graphics::Editor::MODEL_PATH_TYPE);
			
			if (payload){
				path = static_cast<char*>(payload->Data);

				try{
					_model = context.assetManager.loadOrGet<Graphics::Model>(path);
				} catch (const std::exception& e){}
			}
			
			ImGui::EndDragDropTarget();
		}
	}

	void Model::pipelineUI(EngineContext& context){
		static char buff[255] = "";

		if (ImGui::InputText("pipeline", buff, sizeof(buff), ImGuiInputTextFlags_EnterReturnsTrue)){
			try{
				_pipeline = context.assetManager.loadOrGet<Graphics::GraphicsPipeline>(buff);
			} catch (const std::exception& e){
				printf("%s\n", e.what());
			}
			buff[0] = '\0';
		}

		if (ImGui::BeginDragDropTarget()){
			std::filesystem::path path;
			const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(Graphics::Editor::GRAPHICS_PIPELINE_PATH_TYPE);
			
			if (payload){
				path = static_cast<char*>(payload->Data);

				try{
					_pipeline = context.assetManager.loadOrGet<Graphics::GraphicsPipeline>(path);
				} catch (const std::exception& e){}
			}
			
			ImGui::EndDragDropTarget();
		}
	}

	void Model::textureUI(EngineContext& context){
		static char buff[255] = "";
		if (ImGui::InputText("texture", buff, sizeof(buff), ImGuiInputTextFlags_EnterReturnsTrue)){
			try{
				_texture = context.assetManager.loadOrGet<Graphics::Texture>(buff);
			} catch (const std::exception& e){
				printf("%s\n", e.what());
			}
			buff[0] = '\0';
		}

		if (ImGui::BeginDragDropTarget()){
			std::filesystem::path path;
			const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(Graphics::Editor::IMAGE_PATH_TYPE);
			
			if (payload){
				path = static_cast<char*>(payload->Data);

				try{
					_texture = context.assetManager.loadOrGet<Graphics::Texture>(path);
				} catch (const std::exception& e){}
			}
			
			ImGui::EndDragDropTarget();
		}
	}
}