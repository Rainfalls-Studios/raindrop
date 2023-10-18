#ifndef __RAINDROP_GRAPHICS_BUFFERS_VERTEX_LAYOUT_HPP__
#define __RAINDROP_GRAPHICS_BUFFERS_VERTEX_LAYOUT_HPP__

#include <Raindrop/Graphics/Buffers/common.hpp>

namespace Raindrop::Graphics::Buffers{
	class VertexLayout{
		public:
			VertexLayout(Context& context);
			~VertexLayout();

			void addAttribute(const std::string &name, uint32_t binding, VkFormat format);
			void removeAttribute(const std::string& name);

			void setBinding(uint32_t binding);
			void setInputRate(VkVertexInputRate inputRate);

			VkFormat attributeFormat(const std::string& name) const;
			std::size_t attributeOffset(const std::string& name) const;
			std::size_t attributeSize(const std::string& name) const;
			uint32_t attributeBinding(const std::string& name) const;

			std::size_t bindingSize(std::size_t binding) const;

			std::size_t size() const;
			uint32_t binding() const;
			VkVertexInputRate inputRate() const;
			std::size_t attributeCount() const;

			std::vector<VkVertexInputAttributeDescription> attributes() const;
			const VkVertexInputBindingDescription* description() const;

			std::vector<std::string> attributesNames() const;

			std::size_t id(const std::string& name) const;

		private:
			struct AttributeData{
				std::size_t id;
				VkVertexInputAttributeDescription description;
			};

			Context& _context;
			std::unordered_map<std::string, AttributeData> _attributes;
			VkVertexInputBindingDescription _description;

			const AttributeData& get(const std::string& name) const;
	};
}

#endif