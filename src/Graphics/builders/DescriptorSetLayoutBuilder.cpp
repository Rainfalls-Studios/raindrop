#include <Raindrop/Graphics/Builders/DescriptorSetLayoutBuilder.hpp>
#include <Raindrop/Graphics/DescriptorSetLayout.hpp>
#include <Raindrop/Graphics/GraphicsContext.hpp>

namespace Raindrop::Graphics::Builders{
	DescriptorSetLayoutBuilder::DescriptorSetLayoutBuilder(){

	}

	DescriptorSetLayoutBuilder::~DescriptorSetLayoutBuilder(){

	}

	std::shared_ptr<DescriptorSetLayout> DescriptorSetLayoutBuilder::build(GraphicsContext& context){
		VkDescriptorSetLayoutCreateInfo info{};
		info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		info.flags = _flags;
		info.bindingCount = static_cast<uint32_t>(_bindings.size());
		info.pBindings = _bindings.data();

		return std::make_shared<DescriptorSetLayout>(context, info);
	}

	void DescriptorSetLayoutBuilder::setFlags(VkDescriptorSetLayoutCreateFlags flags){
		_flags = flags;
	}

	void DescriptorSetLayoutBuilder::pushBinding(VkDescriptorSetLayoutBinding& binding){
		_bindings.push_back(binding);
	}

	void DescriptorSetLayoutBuilder::loadFromXML(tinyxml2::XMLElement* element, GraphicsContext& context){
		const char* flags;
		if (element->QueryStringAttribute("Flags", &flags) == tinyxml2::XML_SUCCESS)
			_flags = strToVkEnumT<VkDescriptorSetLayoutCreateFlags>(flags);
		
		if (auto poolsElement = element->FirstChildElement("Bindings")){
			for (auto bindingElement = element->FirstChildElement("Binding"); bindingElement!=nullptr; bindingElement = bindingElement->NextSiblingElement("Binding")){
				loadBindingFromXML(bindingElement, context);
			}
		}
	}

	void DescriptorSetLayoutBuilder::loadBindingFromXML(tinyxml2::XMLElement* element, GraphicsContext& context){
		VkDescriptorSetLayoutBinding binding{};

		binding.binding = static_cast<uint32_t>(element->IntAttribute("Binding"));
		binding.descriptorCount = static_cast<uint32_t>(element->IntAttribute("Count"));
		
		const char* type;
		if (element->QueryStringAttribute("Type", &type) == tinyxml2::XML_SUCCESS)
			binding.descriptorType = strToVkEnumT<VkDescriptorType>(type);
		
		
		const char* stages;
		if (element->QueryStringAttribute("Stages", &stages) == tinyxml2::XML_SUCCESS)
			binding.stageFlags = strToVkEnumT<VkShaderStageFlags>(stages);

		if (binding.descriptorType == VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER || binding.descriptorType == VK_DESCRIPTOR_TYPE_SAMPLER){

			_samplers.resize(_samplers.size() + binding.descriptorCount);
			int start = _samplers.size()-1;
			binding.pImmutableSamplers = &_samplers[start];

			int i=0;
			for (auto samplerElement = element->FirstChildElement("Sampler"); samplerElement!=nullptr; samplerElement = samplerElement->NextSiblingElement("Sampler")){
				const char* id;
				if (samplerElement->QueryStringAttribute("ID", &id) == tinyxml2::XML_SUCCESS)
					_samplers[start + i] = reinterpret_cast<VkSampler>(context.gRegistry[id]);
				i++;
			}
		}

		_bindings.push_back(binding);
	}

}
