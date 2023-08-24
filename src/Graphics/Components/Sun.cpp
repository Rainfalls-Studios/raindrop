#include <Raindrop/Graphics/Components/Sun.hpp>
#include <Raindrop/Graphics/ShadowMap/Sun/ShadowMap.hpp>
#include <Raindrop/Graphics/GraphicsContext.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Raindrop::Graphics::Components{
	Sun::Sun(GraphicsContext& context) : _context{context}{
		_color = glm::vec3(1.f);
		_shadowMap = nullptr;
		_descriptorSet = VK_NULL_HANDLE;
		_intensity = 0.f;
	}

	Sun::~Sun(){
		destroyShadowMap();
	}

	bool Sun::hasShadowMap() const{
		return _shadowMap != nullptr;
	}

	const glm::vec3 Sun::color() const{
		return _color;
	}

	const ShadowMap::Sun::ShadowMap& Sun::shadowMap() const{
		return *_shadowMap;
	}

	void Sun::UI(Core::EngineContext& context){
		ImGui::PushID(this);
		
		if (ImGui::TreeNode("Sun")){
			ImGui::DragFloat("intensity", &_intensity);
			ImGui::ColorPicker3("color", glm::value_ptr(_color));
			
			bool hasShadow = hasShadowMap();
			if (ImGui::Checkbox("Shadow", &hasShadow)){
				if (hasShadow){
					createShadowMap();
				} else {
					destroyShadowMap();
				}
			}

			ImGui::TreePop();
		}
		ImGui::PopID();
	}
	
	void Sun::createShadowMap(){
		_shadowMap = std::make_unique<ShadowMap::Sun::ShadowMap>(_context);
		createDescriptorSet();	
		updateDescriptorSet();
	}

	void Sun::destroyShadowMap(){
		_shadowMap.reset();
		destroyDescriptorSet();
	}

	void Sun::updateDescriptorSet(){
		if (!_shadowMap || !_descriptorSet) return;

		VkDescriptorImageInfo imageInfo = _shadowMap->getAttachmentInfo();
		VkWriteDescriptorSet write = {};
		
		write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		write.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		write.dstBinding = 0;
		write.pImageInfo = &imageInfo;
		write.descriptorCount = 1;
		write.dstSet = _descriptorSet;

		vkUpdateDescriptorSets(_context.device.get(), 1, &write, 0, nullptr);
	}

	void Sun::createDescriptorSet(){
		auto pool = _context.descriptorPool.get();
		auto device = _context.device.get();

		VkDescriptorSetLayout layout = _context.layouts.sunShadowMapLayout;

		VkDescriptorSetAllocateInfo info = {};
		info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		info.descriptorSetCount = 1;
		info.descriptorPool = pool;
		info.pSetLayouts = &layout;
		
		if (vkAllocateDescriptorSets(device, &info, &_descriptorSet) != VK_SUCCESS){
			throw std::runtime_error("Failed to allocate descriptor set");
		}
	}

	void Sun::destroyDescriptorSet(){
		auto pool = _context.descriptorPool.get();
		auto device = _context.device.get();
		
		if (_descriptorSet) vkFreeDescriptorSets(device, pool, 1, &_descriptorSet);
	}

	VkDescriptorSet Sun::descriptorSet() const{
		return _descriptorSet;
	}

	float Sun::intensity() const{
		return _intensity;
	}
}