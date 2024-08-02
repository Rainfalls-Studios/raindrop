#include <Raindrop/Graphics/VertexLayout.hpp>
#include <Raindrop/Graphics/Context.hpp>
#include <Raindrop/Graphics/Formats.hpp>

namespace Raindrop::Graphics{
	VertexLayout::VertexLayout() noexcept{}

	VertexLayout::Binding VertexLayout::addBinding(const std::string& name, const VkMemoryPropertyFlags& memProperties, std::uint32_t binding){
		if (binding == BINDING_AUTO){
			binding = _info.bindings.size();
		}

		auto& info = _info.bindings.emplace_back();
		info.binding = binding;
		info.memoryProperties = memProperties;
		info.name = name;
		info.stride = 0;
		info.nextLocation = 0;

		return std::move(Binding(info, _info));
	}

	VertexLayout& VertexLayout::addAttribute(Binding& binding, const std::string& name, const VkFormat& format, std::uint32_t location){
		auto& bindingInfo = binding._info;

		auto& info = _info.attributes.emplace_back();
		info.binding = bindingInfo.binding;
		info.format = format;
		info.location = location == LOCATION_AUTO ? bindingInfo.nextLocation++ : location;
		info.name = name;
		info.offset = bindingInfo.stride;

		bindingInfo.stride += Formats::getSize(format);

		mapAttributes();

		return *this;
	}

	VertexLayout::Attribute::Attribute(AttributeInfo& info) noexcept : _info{info}{}

	VertexLayout::Attribute& VertexLayout::Attribute::setName(const std::string& name){
		_info.name = name;
		return *this;
	}

	VertexLayout::Attribute& VertexLayout::Attribute::setOffset(const std::uint32_t& offset){
		_info.offset = offset;
		return *this;
	}

	VertexLayout::Attribute& VertexLayout::Attribute::setFormat(const VkFormat& format){
		_info.format = format;
		return *this;
	}

	VertexLayout::Attribute& VertexLayout::Attribute::setBinding(const std::uint32_t& binding){
		_info.binding = binding;
		return *this;
	}

	VertexLayout::Attribute& VertexLayout::Attribute::setLocation(const std::uint32_t& location){
		_info.location = location;
		return *this;
	}

	const VertexLayout::AttributeInfo& VertexLayout::Attribute::get() const noexcept{
		return _info;
	}


	VertexLayout::Binding::Binding(BindingInfo& info, Info& data) noexcept :
		_info{info},
		_data{data}
	{}

	VertexLayout::Binding& VertexLayout::Binding::setName(const std::string& name){
		_info.name = name;
		return *this;
	}
	
	VertexLayout::Binding& VertexLayout::Binding::setMemoryProperties(const VkMemoryPropertyFlags& flags){
		_info.memoryProperties = flags;
		return *this;
	}

	const VertexLayout::BindingInfo& VertexLayout::Binding::get() const noexcept{
		return _info;
	}

	void VertexLayout::mapAttributes(){
		_nameToAttributesMap.clear();

		for (auto& attribute : _info.attributes){
			_nameToAttributesMap[attribute.name] = &attribute;
		}
	}

	const VertexLayout::AttributeInfo& VertexLayout::get(const std::string& name) const{
		return *_nameToAttributesMap.at(name);
	}

}