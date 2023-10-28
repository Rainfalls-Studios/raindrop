#ifndef __RAINDROP_GRAPHICS_BUFFERS_VERTEX_LAYOUT_HPP__
#define __RAINDROP_GRAPHICS_BUFFERS_VERTEX_LAYOUT_HPP__

#include <Raindrop/Graphics/Buffers/common.hpp>

namespace Raindrop::Graphics::Buffers{
	class VertexLayout{
		friend class Vertex;
		public:
			VertexLayout(Context& context);
			~VertexLayout();

			VertexLayout(const VertexLayout& other);
			const VertexLayout& operator=(const VertexLayout& other) = delete;

			bool operator==(const VertexLayout& other) const;
			bool operator!=(const VertexLayout& other) const;

			std::size_t addAttribute(const std::string &name, std::size_t binding, VkFormat format);
			void removeAttribute(std::size_t id);
			void removeAttribute(const std::string& name);

			std::size_t attributeSize(std::size_t id) const;
			std::size_t attributeSize(const std::string& name) const;

			const std::string& attributeName(std::size_t id) const;
			std::size_t attributeId(const std::string& name) const;

			std::size_t attributeOffset(std::size_t id) const;
			std::size_t attributeOffset(const std::string& name) const;

			std::size_t attributeBinding(std::size_t id) const;
			std::size_t attributeBinding(const std::string& name) const;

			//  BINDING
			void setBindingInputRate(std::size_t binding, VkVertexInputRate inputRate);
			VkVertexInputRate bindingInputRate(std::size_t binding) const;
			std::size_t bindingSize(std::size_t binding) const;

			const VkVertexInputAttributeDescription* attributeDescriptions() const;
			const VkVertexInputBindingDescription* bindingDescriptions() const;

			std::size_t attributeCount() const;
			std::size_t bindingCount() const;
			std::size_t attributeCountAtBinding(std::size_t binding) const;

			std::size_t size() const;

		private:
			Context& _context;

			std::vector<VkVertexInputAttributeDescription> _attributes;
			std::vector<VkVertexInputBindingDescription> _bindings;

			std::unordered_map<std::string, std::size_t> _nameToAttributeID;

			const VkVertexInputBindingDescription& getBinding(std::size_t binding) const;
			VkVertexInputBindingDescription& getBinding(std::size_t binding);
	};
}

#endif