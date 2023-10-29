// #include <Raindrop/Graphics/DescriptorSetLayouts.hpp>
// #include <Raindrop/Graphics/GraphicsContext.hpp>

// namespace Raindrop::Graphics{
// 	DescriptorLayouts::DescriptorLayouts(GraphicsContext& context) :
// 		_context{context},
// 		scene{context}{
// 		createLightsLayouts();
// 	}

// 	DescriptorLayouts::~DescriptorLayouts(){
// 		destroyLightsLayouts();
// 	}
	
// 	void DescriptorLayouts::createLightsLayouts(){
// 		VkDescriptorSetLayoutBinding binding = {};
// 		binding.binding = 0;
// 		binding.descriptorCount = 1;
// 		binding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
// 		binding.pImmutableSamplers = nullptr;
// 		binding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

// 		VkDescriptorSetLayoutCreateInfo info = {};
// 		info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
// 		info.bindingCount = 1;
// 		info.pBindings = &binding;
		
// 		auto device = _context.device().get();
// 		auto allocationCallbacks = _context.allocationCallbacks;
		
// 		if (vkCreateDescriptorSetLayout(device, &info, allocationCallbacks, &sunShadowMapLayout) != VK_SUCCESS){
// 			CLOG(ERROR, "Engine.Graphics") << "Failed to create lights descriptor set layouts";
// 			throw std::runtime_error("failed to create light descriptor set layouts");
// 		}

// 		lightPointShadowMapLayout = sunShadowMapLayout;
// 		spotlightShadowMapLayout = sunShadowMapLayout;
// 	}

// 	void DescriptorLayouts::destroyLightsLayouts(){
// 		auto device = _context.device().get();
// 		auto allocationCallbacks = _context.allocationCallbacks;

// 		if (sunShadowMapLayout) vkDestroyDescriptorSetLayout(device, sunShadowMapLayout, allocationCallbacks);

// 		// since they are all the same
// 		// if (lightPointShadowMapLayout) vkDestroyDescriptorSetLayout(device, lightPointShadowMapLayout, allocationCallbacks);
// 		// if (spotlightShadowMapLayout) vkDestroyDescriptorSetLayout(device, spotlightShadowMapLayout, allocationCallbacks);
// 	}
// }