#include <Raindrop/Graphics/builders/DescriptorPoolBuilder.hpp>
#include <Raindrop/Graphics/GraphicsContext.hpp>
#include <Raindrop/Graphics/DescriptorPool.hpp>

namespace Raindrop::Graphics::Builders{
	DescriptorPoolBuilder::DescriptorPoolBuilder(){

	}

	DescriptorPoolBuilder::~DescriptorPoolBuilder(){
		
	}

	void DescriptorPoolBuilder::setFlags(VkDescriptorPoolCreateFlags flags){
		_flags = flags;
	}

	void DescriptorPoolBuilder::setMaxSets(uint32_t max){
		_maxSets = max;
	}

	void DescriptorPoolBuilder::pushPoolSize(VkDescriptorPoolSize size){
		_poolSizes.push_back(size);
	}

	std::shared_ptr<DescriptorPool> DescriptorPoolBuilder::build(GraphicsContext& context){
		VkDescriptorPoolCreateInfo info{};
		info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		info.flags = _flags;
		info.maxSets = _maxSets;
		info.pPoolSizes = _poolSizes.data();
		info.poolSizeCount = static_cast<uint32_t>(_poolSizes.size());

		return std::make_shared<DescriptorPool>(context, info);
	}

	void DescriptorPoolBuilder::loadFromXML(tinyxml2::XMLElement* element){
		const char* flags;
		if (element->QueryStringAttribute("Flags", &flags) == tinyxml2::XML_SUCCESS)
			_flags = strToVkEnumT(flags, 0);
		
		_maxSets = element->IntAttribute("MaxSets");

		if (auto poolsElement = element->FirstChildElement("Pools")){
			for (auto poolSizeElement = element->FirstChildElement("PoolSize"); poolSizeElement!=nullptr; poolSizeElement = poolSizeElement->NextSiblingElement("PoolSize")){
				loadPoolFromXML(poolSizeElement);
			}
		}
	}

	void DescriptorPoolBuilder::loadPoolFromXML(tinyxml2::XMLElement* element){
		VkDescriptorPoolSize poolSize{};

		const char* type;
		if (element->QueryStringAttribute("Type", &type) == tinyxml2::XML_SUCCESS)
			poolSize.type = strToVkEnumT(type, 0);
		
		poolSize.descriptorCount = element->IntAttribute("DescriptorCount");

		_poolSizes.push_back(poolSize);
	}
}