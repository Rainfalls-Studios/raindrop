#ifndef __RAINDROP_GRAPHICS_BUILDERS_DESCRIPTOR_SET_LAYOUT_BUILDER_HPP__
#define __RAINDROP_GRAPHICS_BUILDERS_DESCRIPTOR_SET_LAYOUT_BUILDER_HPP__

#include <Raindrop/Graphics/common.hpp>

//TODO: Add 'createFromXML' - https://trello.com/c/fnHsUiAp/2-add-createfromxml
namespace Raindrop::Graphics::Builders{
	class DescriptorSetLayoutBuilder{
		public:
			DescriptorSetLayoutBuilder();
			~DescriptorSetLayoutBuilder();

			std::shared_ptr<DescriptorSetLayout> build(GraphicsContext& context);

			void setFlags(VkDescriptorSetLayoutCreateFlags flags);
			void pushBinding(VkDescriptorSetLayoutBinding& binding);

			void loadFromXML(tinyxml2::XMLElement* element, GraphicsContext& context);

		private:
			VkDescriptorSetLayoutCreateFlags _flags = 0;
			std::vector<VkDescriptorSetLayoutBinding> _bindings;
			std::vector<VkSampler> _samplers;

			void loadBindingFromXML(tinyxml2::XMLElement* element, GraphicsContext& context);
	};
}

#endif