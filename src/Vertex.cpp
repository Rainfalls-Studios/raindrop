#include <Raindrop/Vertex.hpp>

namespace Raindrop{
	Vertex::Layout::Attribute::Attribute(const std::string& name, const Format& format, std::size_t size, std::size_t offset) : 
		name{name},
		format{format},
		size{size},
		offset{offset}
	{}

	Vertex::Layout::Layout() : 
		_stride{0},
		_isStrideCustom{false}
	{}

	Vertex::Layout& Vertex::Layout::addAttribute(Attribute attribute){
		if (attribute.size == Attribute::SIZE_AUTO){
			attribute.size = attribute.format.getSize();
		}

		if (attribute.offset == Attribute::OFFSET_AUTO){
			attribute.offset = _stride;
		}

		_attributes.push_back(attribute);

		if (_isStrideCustom == false){
			_stride += attribute.size;
		}
		return *this;
	}

	Vertex::Layout& Vertex::Layout::setStride(const std::size_t stride){
		_stride = stride;
		_isStrideCustom = true;
		return *this;
	}


	const std::vector<Vertex::Layout::Attribute>& Vertex::Layout::getAttributes() const noexcept{
		return _attributes;
	}

	const std::size_t& Vertex::Layout::getStride() const noexcept{
		return _stride;
	}
}