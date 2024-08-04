#include <Raindrop/Graphics/VertexLayout.hpp>
#include <Raindrop/Graphics/Context.hpp>
#include <Raindrop/Graphics/Formats.hpp>

namespace Raindrop::Graphics{
	VertexLayout::VertexLayout() noexcept{}

	VertexLayout::Binding VertexLayout::addBinding(const std::string& name, const VkVertexInputRate& rate, const VkMemoryPropertyFlags& memProperties, std::uint32_t binding){
		if (binding == BINDING_AUTO){
			binding = _bindings.size();
		}

		BindingInfo info{
			.binding = binding,
			.stride = 0,
			.memoryProperties = memProperties,
			.name = name,
			.rate = rate
		};

		auto& ref = _bindings[name] = info;

		return std::move(Binding(ref));
	}

	VertexLayout& VertexLayout::addAttribute(Binding& binding, const std::string& name, const VkFormat& format, std::uint32_t location){
		auto& bindingInfo = binding._info;

		AttributeInfo info{
			.format = format,
			.offset = bindingInfo.stride,
			.binding = bindingInfo.binding,
			.location = location == LOCATION_AUTO ? static_cast<uint32_t>(bindingInfo.attributes.size()) : location,
			.name = name
		};

		bindingInfo.attributes[name] = info;
		bindingInfo.stride += Formats::getSize(format);

		return *this;
	}
	
	VertexLayout::Binding::Binding(BindingInfo& info) noexcept : _info{info}{}
	
	VertexLayout::Binding& VertexLayout::Binding::setMemoryProperties(const VkMemoryPropertyFlags& flags){
		_info.memoryProperties = flags;
		return *this;
	}

	VertexLayout::Binding& VertexLayout::Binding::setInputRate(const VkVertexInputRate& rate){
		_info.rate = rate;
		return *this;
	}

	const VertexLayout::BindingInfo& VertexLayout::Binding::get() const noexcept{
		return _info;
	}

	std::vector<VkVertexInputBindingDescription> VertexLayout::getBindingDescriptions() const{
		std::vector<VkVertexInputBindingDescription> descriptions;
		descriptions.reserve(_bindings.size());

		for (auto& binding : _bindings){
			VkVertexInputBindingDescription description{
				.binding = binding.second.binding,
				.stride = binding.second.stride,
				.inputRate = binding.second.rate
			};

			descriptions.push_back(description);
		}

		return std::move(descriptions);
	}

	std::vector<VkVertexInputAttributeDescription> VertexLayout::getAttributeDescriptions() const {
		std::vector<VkVertexInputAttributeDescription> descriptions;

		for (auto& binding : _bindings){
			for (auto& attribute : binding.second.attributes){
				VkVertexInputAttributeDescription description{
					.location = attribute.second.location,
					.binding = attribute.second.binding,
					.format = attribute.second.format,
					.offset = attribute.second.offset
				};

				descriptions.push_back(description);
			}
		}

		return std::move(descriptions);
	}

	VertexLayout::AttributeInfo& VertexLayout::BindingInfo::operator[](const std::string& name){
		return attributes.at(name);
	}

	const VertexLayout::AttributeInfo& VertexLayout::BindingInfo::operator[](const std::string& name) const{
		return attributes.at(name);
	}

	VertexLayout::BindingInfo& VertexLayout::operator[](const std::string& name){
		return _bindings.at(name);
	}

	const VertexLayout::BindingInfo& VertexLayout::operator[](const std::string& name) const{
		return _bindings.at(name);
	}

	std::size_t VertexLayout::getBindingCount() const noexcept{
		return _bindings.size();
	}

	const std::unordered_map<std::string, VertexLayout::BindingInfo>& VertexLayout::getBindings() const noexcept{
		return _bindings;
	}

	// std::vector<const BindingInfo&> VertexLayout::getVertexRateBindings() const noexcept{
	// 	std::vector<const BindingInfo&> bindings;
	// 	for (auto& b : _bindings){
	// 		if (b.second.rate == VK_VERTEX_INPUT_RATE_VERTEX){
	// 			bindings.push_back(b.second);
	// 		}
	// 	}
	// 	return bindings;
	// }
}