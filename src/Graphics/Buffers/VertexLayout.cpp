#include <Raindrop/Graphics/Buffers/VertexLayout.hpp>
#include <Raindrop/Graphics/Buffers/Context.hpp>

namespace Raindrop::Graphics::Buffers{
	VertexLayout::VertexLayout(Context& context) : _context{context}{}
	VertexLayout::~VertexLayout(){}
	
	VertexLayout::VertexLayout(const VertexLayout& other) : 
		_context{other._context},
		_attributes{other._attributes},
		_bindings{other._bindings},
		_nameToAttributeID{other._nameToAttributeID}
	{}

	std::size_t VertexLayout::addAttribute(const std::string &name, std::size_t binding, VkFormat format){
		if (name.empty()){
			throw std::runtime_error("Cannot have an unnamed vertex attribute");
		}

		if (binding > _context.graphics().internal().limits().maxVertexInputBindings){
			_context.logger().error("TVertex binding out of limitation range (%d > %d)", binding, _context.graphics().internal().limits().maxVertexInputBindings);
			throw std::out_of_range("Vertex binding out of limitation range");
		}

		if (_bindings.size() < binding+1){
			_bindings.resize(binding+1);
		}

		VkVertexInputAttributeDescription attribute;
		attribute.binding = static_cast<uint32_t>(binding);
		attribute.location = static_cast<uint32_t>(attributeCountAtBinding(binding));
		attribute.format = format;
		
		uint32_t& bindingStride = _bindings[binding].stride;
		attribute.offset = bindingStride;
		bindingStride += _context.graphics().utils().formats().size(format);

		std::size_t id = _attributes.size();
		_attributes.push_back(attribute);

		_nameToAttributeID[name] = id;
		return id;
	}

	void VertexLayout::removeAttribute(std::size_t id){
		for (const auto& pair : _nameToAttributeID){
			if (pair.second != id) continue;
			_nameToAttributeID.erase(pair.first);
			break;
		}

		uint32_t binding = _attributes[id].binding;
		_attributes.erase(_attributes.begin() + id);

		uint32_t offset = 0;
		for (auto& attribute : _attributes){
			if (attribute.binding == binding){
				attribute.offset = offset;
				offset += _context.graphics().utils().formats().size(attribute.format);
			}
		}

		_bindings[static_cast<std::size_t>(binding)].stride = offset;
	}

	void VertexLayout::removeAttribute(const std::string& name){
		removeAttribute(attributeId(name));
	}

	std::size_t VertexLayout::attributeSize(std::size_t id) const{
		return _context.graphics().utils().formats().size(_attributes[id].format);
	}

	std::size_t VertexLayout::attributeSize(const std::string& name) const{
		return attributeSize(attributeId(name));
	}

	const std::string& VertexLayout::attributeName(std::size_t id) const{
		for (const auto& pair : _nameToAttributeID){
			if (pair.second == id) return pair.first;
		}
		throw std::runtime_error("invalid ID");
	}

	std::size_t VertexLayout::attributeId(const std::string& name) const{
		auto it = _nameToAttributeID.find(name);
		if (it == _nameToAttributeID.end()){
			return std::numeric_limits<std::size_t>::quiet_NaN();
		}
		return it->second;
	}

	std::size_t VertexLayout::attributeOffset(std::size_t id) const{
		return _attributes[id].offset;
	}

	std::size_t VertexLayout::attributeOffset(const std::string& name) const{
		return attributeOffset(attributeId(name));
	}
	
	std::size_t VertexLayout::attributeBinding(std::size_t id) const{
		return static_cast<std::size_t>(_attributes[id].binding);
	}

	std::size_t VertexLayout::attributeBinding(const std::string& name) const{
		return attributeBinding(attributeId(name));
	}

	//  BINDING
	void VertexLayout::setBindingInputRate(std::size_t binding, VkVertexInputRate inputRate){
		getBinding(binding).inputRate = inputRate;
	}

	VkVertexInputRate VertexLayout::bindingInputRate(std::size_t binding) const{
		return getBinding(binding).inputRate;
	}

	std::size_t VertexLayout::bindingSize(std::size_t binding) const{
		return static_cast<std::size_t>(getBinding(binding).stride);
	}

	const VkVertexInputAttributeDescription* VertexLayout::attributeDescriptions() const{
		return _attributes.data();
	}

	const VkVertexInputBindingDescription* VertexLayout::bindingDescriptions() const{
		return _bindings.data();
	}

	std::size_t VertexLayout::attributeCount() const{
		return _attributes.size();
	}

	std::size_t VertexLayout::bindingCount() const{
		return _bindings.size();
	}

	std::size_t VertexLayout::attributeCountAtBinding(std::size_t binding) const{
		std::size_t size = 0;
		for (auto& attribute : _attributes){
			if (attribute.binding == static_cast<uint32_t>(binding)) size++;
		}
		return size;
	}
	
	const VkVertexInputBindingDescription& VertexLayout::getBinding(std::size_t binding) const{
		if (binding > _bindings.size()){
			throw std::out_of_range("binding ID out of range");
		}

		return _bindings[binding];
	}

	VkVertexInputBindingDescription& VertexLayout::getBinding(std::size_t binding){
		if (binding > _bindings.size()){
			throw std::out_of_range("binding ID out of range");
		}

		return _bindings[binding];
	}
	
	bool VertexLayout::operator==(const VertexLayout& other) const{
		return !(*this != other);
	}

	bool VertexLayout::operator!=(const VertexLayout& other) const{
		if (_attributes.size() != other._attributes.size() || _bindings.size() != other._bindings.size()) return true;
		if (std::memcmp(_attributes.data(), other._attributes.data(), sizeof(_attributes[0]) * _attributes.size()) != 0) return true;
		if (std::memcmp(_bindings.data(), other._bindings.data(), sizeof(_bindings[0]) * _bindings.size()) != 0) return true;
		return false;
	}

	std::size_t VertexLayout::size() const{
		std::size_t size = 0;
		for (const auto& binding : _bindings){
			size += static_cast<std::size_t>(binding.stride);
		}
		return size;
	}
}