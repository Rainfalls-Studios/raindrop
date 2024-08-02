#ifndef __RAINDROP_GRAPHICS_VERTEX_LAYOUT_HPP__
#define __RAINDROP_GRAPHICS_VERTEX_LAYOUT_HPP__

#include "types.hpp"
#include "pch.pch"
#include "Utils.hpp"

namespace Raindrop::Graphics{
	class VertexLayout{
		private:
			struct Info;

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
				std::uint32_t nextLocation;
				VkMemoryPropertyFlags memoryProperties;
				std::string name;
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
					Binding(BindingInfo& info, Info& data) noexcept;
					~Binding() = default;

					Binding& setName(const std::string& name);
					Binding& setMemoryProperties(const VkMemoryPropertyFlags& flags);

					const BindingInfo& get() const noexcept;

				private:
					BindingInfo& _info;
					Info& _data;
			};

			static constexpr std::uint32_t BINDING_AUTO = UINT32_MAX;
			static constexpr std::uint32_t LOCATION_AUTO = UINT32_MAX;

			VertexLayout() noexcept;
			~VertexLayout() = default;

			Binding addBinding(const std::string& name, const VkMemoryPropertyFlags& memProperties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, std::uint32_t binding = BINDING_AUTO);

			VertexLayout& addAttribute(Binding& binding, const std::string& name, const VkFormat& format, std::uint32_t location = LOCATION_AUTO);
			
			template<typename T>
			inline VertexLayout& addAttribute(Binding& binding, const std::string& name, std::uint32_t location = LOCATION_AUTO){
				return addAttribute(binding, name, Utils::typeToFormat<T>(), location);
			}
			
			const AttributeInfo& get(const std::string& name) const;

		private:
			struct Info{
				std::vector<BindingInfo> bindings;
				std::vector<AttributeInfo> attributes;	
			} _info;

			std::unordered_map<std::string, AttributeInfo*> _nameToAttributesMap;

			void mapAttributes();

	};
}

#endif