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

				/**
				 * @brief Access the attribute named 'name'
				 * 
				 * @param name The name of the attribute to access
				 * @return AttributeInfo& 
				 */
				AttributeInfo& operator[](const std::string& name);

				
				/**
				 * @brief Access the attribute named 'name'
				 * 
				 * @param name The name of the attribute to access
				 * @return const AttributeInfo& 
				 */
				const AttributeInfo& operator[](const std::string& name) const;
			};

			class Binding{
				friend class VertexLayout;
				public:	
					Binding(BindingInfo& info) noexcept;
					~Binding() = default;

					/**
					 * @brief Set the wanted memory properties of the binding
					 * 
					 * @param flags The memory properties flags
					 * @return Binding& 
					 */
					Binding& setMemoryProperties(const VkMemoryPropertyFlags& flags);

					/**
					 * @brief Set the input rate of the binding
					 * 
					 * @param rate The binding's input rate
					 * @return Binding& 
					 */
					Binding& setInputRate(const VkVertexInputRate& rate);

					/**
					 * @brief Get the binding raw info
					 * 
					 * @return const BindingInfo& 
					 */
					const BindingInfo& get() const noexcept;

				private:
					BindingInfo& _info;
			};

			
			static constexpr std::uint32_t BINDING_AUTO = UINT32_MAX;
			static constexpr std::uint32_t LOCATION_AUTO = UINT32_MAX;

			VertexLayout() noexcept;
			~VertexLayout() = default;
			
			/**
			 * @brief Add a binding to the vertex layout.
			 * 
			 * @param name The name of the binding
			 * @param rate the input rate for this binding (per vertex or per instance)
			 * @param memProperties The required memory properties for this binding (eg. host visible for dynamic updates of the buffer)
			 * @param binding the binding index
			 * @return Binding 
			 */
			Binding addBinding(const std::string& name, const VkVertexInputRate& rate = VK_VERTEX_INPUT_RATE_VERTEX, const VkMemoryPropertyFlags& memProperties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, std::uint32_t binding = BINDING_AUTO);

			/**
			 * @brief Add a new attribute to the binding
			 * 
			 * @param binding The binding to add the attribute to
			 * @param name The name of the attribute
			 * @param format The format of the data
			 * @param location The location of the attribute within the binding
			 * @return VertexLayout& 
			 */
			VertexLayout& addAttribute(Binding& binding, const std::string& name, const VkFormat& format, std::uint32_t location = LOCATION_AUTO);
			
			/**
			 * @brief Add a new attribute to the binding
			 * 
			 * @tparam T The type of the attribute to add
			 * @param bindihng The binding to add the attribute to
			 * @param name The name of the attribute
			 * @param location The location of the attribute within the binding
			 * @return VertexLayout& 
			 */
			template<typename T>
			inline VertexLayout& addAttribute(Binding& binding, const std::string& name, std::uint32_t location = LOCATION_AUTO){
				return addAttribute(binding, name, Utils::typeToFormat<T>(), location);
			}

			/**
			 * @brief Get the bindings as an array of vulkan binding description
			 * 
			 * @return std::vector<VkVertexInputBindingDescription> 
			 */
			std::vector<VkVertexInputBindingDescription> getBindingDescriptions() const;

			/**
			 * @brief Get the attributes as an array of vulkan attribute description
			 * 
			 * @return std::vector<VkVertexInputAttributeDescription> 
			 */
			std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions() const;

			/**
			 * @brief Access the binding named 'name'
			 * 
			 * @param name The name of the binding to access
			 * @return BindingInfo& 
			 */
			BindingInfo& operator[](const std::string& name);

			/**
			 * @brief Access the binding named 'name'
			 * 
			 * @param name The name of the binding to access
			 * @return const BindingInfo& 
			 */
			const BindingInfo& operator[](const std::string& name) const;

			/**
			 * @brief Get the total number of bindings in the vertex layout
			 * 
			 * @return std::size_t 
			 */
			std::size_t getBindingCount() const noexcept;

			/**
			 * @brief Get the bindings as an unordered map mapping the name of the bindings to their data
			 * 
			 * @return const std::unordered_map<std::string, BindingInfo>& 
			 */
			const std::unordered_map<std::string, BindingInfo>& getBindings() const noexcept;

		private:
			std::unordered_map<std::string, BindingInfo> _bindings;
	};
}

#endif