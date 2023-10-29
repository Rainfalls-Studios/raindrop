// #include <Raindrop/Graphics/SceneDescriptorSetLayout.hpp>
// #include <Raindrop/Graphics/GraphicsContext.hpp>
// #include "configurations/Scene.hpp"

// namespace Raindrop::Graphics{
// 	SceneDescriptorSetLayout::SceneDescriptorSetLayout(GraphicsContext& context) :
// 		_context{context},
// 		_setLayout{VK_NULL_HANDLE}{
// 		createSetLayout();
// 	}

// 	SceneDescriptorSetLayout::~SceneDescriptorSetLayout(){
// 		destroySetLayout();
// 	}

// 	VkDescriptorSetLayout SceneDescriptorSetLayout::get() const{
// 		return _setLayout;
// 	}
	
// 	void SceneDescriptorSetLayout::createSetLayout(){
// 		destroySetLayout();

// 		auto device = _context.device().get();
// 		auto allocationCallbacks = _context.allocationCallbacks;

// 		std::vector<VkDescriptorSetLayoutBinding> bindings(attachments.size());
// 		for (int i=0; i<bindings.size(); i++){
// 			auto& binding = bindings[i];

// 			binding.binding = i;
// 			binding.descriptorCount = 1;
// 			binding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
// 			binding.pImmutableSamplers = nullptr;
// 			binding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
// 		}

// 		VkDescriptorSetLayoutCreateInfo info = {};
// 		info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
// 		info.bindingCount = static_cast<uint32_t>(bindings.size());
// 		info.pBindings = bindings.data();

// 		if (vkCreateDescriptorSetLayout(device, &info, allocationCallbacks, &_setLayout) != VK_SUCCESS){
// 			throw std::runtime_error("failed to create scene descriptor set layouts");
// 		}
// 	}

// 	void SceneDescriptorSetLayout::destroySetLayout(){
// 		auto device = _context.device().get();
// 		auto allocationCallbacks = _context.allocationCallbacks;

// 		if (_setLayout) vkDestroyDescriptorSetLayout(device, _setLayout, allocationCallbacks);

// 		_setLayout = VK_NULL_HANDLE;
// 	}
// }