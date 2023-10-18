#include <Raindrop/Graphics/Buffers/VertexLayout.hpp>
#include <Raindrop/Graphics/Buffers/Context.hpp>

namespace Raindrop::Graphics::Buffers{
	VertexLayout::VertexLayout(Context& context) : _context{context}, _description{}{}
	VertexLayout::~VertexLayout(){}

	void VertexLayout::addAttribute(const std::string &name, uint32_t binding, VkFormat format){
		VkVertexInputAttributeDescription description;

		description.location = static_cast<uint32_t>(_attributes.size());
		description.format = format;
		description.binding = binding;
		description.offset = _description.stride;

		_description.stride += static_cast<uint32_t>(_context.graphics.utils.formats.size(format));

		AttributeData attribute;
		attribute.description = description;
		attribute.id = _attributes.size();

		_attributes[name] = attribute;
	}

	void VertexLayout::removeAttribute(const std::string& name){
		auto it = _attributes.find(name);
		if (it == _attributes.end());

		_attributes.erase(name);

		uint32_t offset = 0;
		std::size_t index = 0;

		for (auto &attribute : _attributes){
			auto& description = attribute.second.description;

			attribute.second.id = index;
			description.offset = offset;

			offset += _context.graphics.utils.formats.size(description.format);
			index++;
		}
		_description.stride = offset;
	}
	
	void VertexLayout::setBinding(uint32_t binding){
		_description.binding = binding;
	}

	void VertexLayout::setInputRate(VkVertexInputRate inputRate){
		_description.inputRate = inputRate;
	}

	VkFormat VertexLayout::attributeFormat(const std::string& name) const{
		return get(name).description.format;

	}

	std::size_t VertexLayout::attributeOffset(const std::string& name) const{
		return get(name).description.offset;
	}

	std::size_t VertexLayout::attributeSize(const std::string& name) const{
		return _context.graphics.utils.formats.size(attributeFormat(name));
	}

	uint32_t VertexLayout::attributeBinding(const std::string& name) const{
		return get(name).description.binding;
	}

	std::size_t VertexLayout::size() const{
		return _description.stride;
	}

	const VertexLayout::AttributeData& VertexLayout::get(const std::string& name) const{
		auto it = _attributes.find(name);
		if (it == _attributes.end()){
			throw std::runtime_error("Could not find attribute");
		}
		return it->second;
	}
	
	std::vector<VkVertexInputAttributeDescription> VertexLayout::attributes() const{
		std::vector<VkVertexInputAttributeDescription> vec(_attributes.size());

		std::size_t i = 0;
		for (const auto& attribute : _attributes){
			vec[i] = attribute.second.description;
			i++;
		}

		return vec;
	}

	const VkVertexInputBindingDescription* VertexLayout::description() const{
		return &_description;
	}

	uint32_t VertexLayout::binding() const{
		return _description.binding;
	}

	VkVertexInputRate VertexLayout::inputRate() const{
		return _description.inputRate;
	}

	std::size_t VertexLayout::id(const std::string& name) const{
		return get(name).id;
	}

	std::size_t VertexLayout::attributeCount() const{
		return _attributes.size();
	}

	std::vector<std::string> VertexLayout::attributesNames() const{
		std::vector<std::string> names(_attributes.size());
		std::size_t i=0;
		for (const auto& a : _attributes){
			names[i] = a.first;
			i++;
		}
		return names;
	}

	std::size_t VertexLayout::bindingSize(std::size_t binding) const{
		std::size_t size = 0;
		for (const auto& attribute : _attributes){
			const auto& description = attribute.second.description;
			if (description.binding != static_cast<uint32_t>(binding)) continue;
			size += _context.graphics.utils.formats.size(description.format);
		}
		return size;
	}
}