#ifndef __RAINDROP_GRAPHICS_VERTEX_LAYOUT_HPP__
#define __RAINDROP_GRAPHICS_VERTEX_LAYOUT_HPP__

#include "types.hpp"
#include "pch.pch"
#include "Utils.hpp"

namespace Raindrop::Graphics{
	class VertexLayout{
		public:
			struct AttributeInfo{
				VkFormat format;
				std::uint32_t offset;
				std::uint32_t binding;
				std::uint32_t location;
				std::string name;
			};

			struct BindingInfo{
				std::uint32_t binding;
				std::uint32_t stride;
				VkMemoryPropertyFlags memoryProperties;
				std::string name;
				VkVertexInputRate rate;

				std::unordered_map<std::string, AttributeInfo>  attributes;

				AttributeInfo& operator[](const std::string& name);
				const AttributeInfo& operator[](const std::string& name) const;
			};

			class Attribute{
				friend class VertexLayout;
				public:
					Attribute(AttributeInfo& info) noexcept;
					~Attribute() = default;

					Attribute& setName(const std::string& name);
					Attribute& setOffset(const std::uint32_t& offset);
					Attribute& setFormat(const VkFormat& format);
					Attribute& setBinding(const std::uint32_t& binding);
					Attribute& setLocation(const std::uint32_t& location);

					const AttributeInfo& get() const noexcept;

				private:
					AttributeInfo& _info;
			};

			class Binding{
				friend class VertexLayout;
				public:	
					Binding(BindingInfo& info) noexcept;
					~Binding() = default;

					Binding& setName(const std::string& name);
					Binding& setMemoryProperties(const VkMemoryPropertyFlags& flags);
					Binding& setInputRate(const VkVertexInputRate& rate);

					const BindingInfo& get() const noexcept;

				private:
					BindingInfo& _info;
			};

			static constexpr std::uint32_t BINDING_AUTO = UINT32_MAX;
			static constexpr std::uint32_t LOCATION_AUTO = UINT32_MAX;

			VertexLayout() noexcept;
			~VertexLayout() = default;

			Binding addBinding(const std::string& name, const VkVertexInputRate& rate = VK_VERTEX_INPUT_RATE_VERTEX, const VkMemoryPropertyFlags& memProperties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, std::uint32_t binding = BINDING_AUTO);

			VertexLayout& addAttribute(Binding& binding, const std::string& name, const VkFormat& format, std::uint32_t location = LOCATION_AUTO);
			
			template<typename T>
			inline VertexLayout& addAttribute(Binding& binding, const std::string& name, std::uint32_t location = LOCATION_AUTO){
				return addAttribute(binding, name, Utils::typeToFormat<T>(), location);
			}

			std::vector<VkVertexInputBindingDescription> getBindingDescriptions() const;
			std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions() const;

			BindingInfo& operator[](const std::string& name);
			const BindingInfo& operator[](const std::string& name) const;

			std::size_t getBindingCount() const noexcept;

		private:
			std::unordered_map<std::string, BindingInfo> _bindings;

	};
}

#endif