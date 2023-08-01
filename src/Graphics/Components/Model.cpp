#include <Raindrop/Graphics/Components/Model.hpp>
#include <Raindrop/Graphics/GraphicsContext.hpp>
#include <Raindrop/Graphics/Editor/DragDropHandler.hpp>
#include <Raindrop/Graphics/Texture.hpp>
#include <Raindrop/Graphics/Model.hpp>

namespace Raindrop::Graphics::Components{
	Model::Model(GraphicsContext& context) : _context{context}{
		VkDescriptorSetLayout layout = reinterpret_cast<VkDescriptorSetLayout>(_context.gRegistry["layout"]);

		VkDescriptorSetAllocateInfo info = {};
		info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		info.descriptorPool = _context.pool;
		info.descriptorSetCount = 1;
		info.pSetLayouts = &layout;

		if (vkAllocateDescriptorSets(_context.device.get(), &info, &_descriptorSet) != VK_SUCCESS){
			CLOG(ERROR, "Engine.Graphics") << "Failed to allocate model descritor set";
			throw std::runtime_error("Failed to allocate model descritor set");
		}
	}

	Model::~Model(){
		vkFreeDescriptorSets(_context.device.get(), _context.pool, 1, &_descriptorSet);
	}
	
	Graphics::Texture* Model::texture() const{
		auto tex = _texture.lock();
		if (tex) return tex.get();
		return nullptr;
	}

	void Model::updateTexture(std::weak_ptr<Graphics::Texture> texture){
		_texture = texture;

		VkDescriptorImageInfo imageInfo;
		if (auto lock = texture.lock()){
			imageInfo = lock->info();
		} else {
			
		}

		VkWriteDescriptorSet write = {};
		
		write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		write.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		write.dstBinding = 0;
		write.pImageInfo = &imageInfo;
		write.descriptorCount = 1;
		write.dstSet = _descriptorSet;

		vkUpdateDescriptorSets(_context.device.get(), 1, &write, 0, nullptr);
	}

	Graphics::Model* Model::model() const{
		auto mod = _model.lock();
		if (mod) return mod.get();
		return nullptr;
	}

	VkDescriptorSet Model::descriptorSet() const{
		return _descriptorSet;
	}
	
	void Model::modelUI(){
		auto& assetManager = _context.context.assetManager;
		static char buff[255] = "";

		if (ImGui::InputText("model", buff, sizeof(buff), ImGuiInputTextFlags_EnterReturnsTrue)){
			try{
				_model = assetManager.loadOrGet<Graphics::Model>(buff);
			} catch (const std::exception& e){
				printf("%s\n", e.what());
			}
			buff[0] = '\0';
		}

		if (ImGui::BeginDragDropTarget()){
			std::filesystem::path path;
			const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(Editor::MODEL_PATH_TYPE);
			
			if (payload){
				path = static_cast<char*>(payload->Data);

				try{
					_model = assetManager.loadOrGet<Graphics::Model>(path);
				} catch (const std::exception& e){}
			}
			
			ImGui::EndDragDropTarget();
		}
	}

	void Model::textureUI(){
		auto& assetManager = _context.context.assetManager;

		static char buff[255] = "";
		if (ImGui::InputText("texture", buff, sizeof(buff), ImGuiInputTextFlags_EnterReturnsTrue)){
			try{
				_texture = assetManager.loadOrGet<Graphics::Texture>(buff);
			} catch (const std::exception& e){
				printf("%s\n", e.what());
			}
			buff[0] = '\0';
		}

		if (ImGui::BeginDragDropTarget()){
			std::filesystem::path path;
			const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(Editor::IMAGE_PATH_TYPE);
			
			if (payload){
				path = static_cast<char*>(payload->Data);

				try{
					updateTexture(assetManager.loadOrGet<Graphics::Texture>(path));
				} catch (const std::exception& e){}
			}
			
			ImGui::EndDragDropTarget();
		}
	}
	
	void Model::UI(Core::EngineContext& context){
		textureUI();
		modelUI();
	}
}