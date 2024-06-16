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


	//--------------------------------------------------------------------
	//-----------------          VERTEX BUFFER           -----------------
	//--------------------------------------------------------------------

	Vertex::Buffer::Buffer(Context& context) : 
		GBuffer(context),
		_layout()
	{}

	const Vertex::Layout& Vertex::Buffer::getLayout(const Layout& layout){
		return _layout;
	}

	void* Vertex::Buffer::getMappedPointer(){
		return GBuffer::getMappedPointer();
	}

	void Vertex::Buffer::allocate(const Layout& layout, const std::size_t& vertexCount, const Usage& usage, const Flags& flags, const GMemory::Type::Flags& memoryFlags, const std::size_t& alignement){
		_layout = layout;
		// GBuffer::allocate(_layout.getStride() * vertexCount, usage | Usage::VERTEX_BUFFER, flags, memoryFlags, alignement);

		usage.operator|(Usage::VERTEX_BUFFER);
		auto a = usage | Usage::VERTEX_BUFFER;
	}

	void Vertex::Buffer::map(const std::size_t& vertexCount, const std::size_t& vertexOffset){
		const std::size_t size = vertexCount == WHOLE_SIZE ? WHOLE_SIZE : vertexCount * _layout.getStride();
		const std::size_t offset = vertexOffset * _layout.getStride();

		GBuffer::map(size, offset);
	}

	void Vertex::Buffer::flush(const std::size_t& vertexCount, const std::size_t& vertexOffset){
		const std::size_t size = vertexCount == WHOLE_SIZE ? WHOLE_SIZE : vertexCount * _layout.getStride();
		const std::size_t offset = vertexOffset * _layout.getStride();

		GBuffer::flush(size, offset);
	}

	void Vertex::Buffer::invalidate(const std::size_t& vertexCount, const std::size_t& vertexOffset){
		const std::size_t size = vertexCount == WHOLE_SIZE ? WHOLE_SIZE : vertexCount * _layout.getStride();
		const std::size_t offset = vertexOffset * _layout.getStride();

		GBuffer::invalidate(size, offset);
	}

	const std::size_t Vertex::Buffer::getVertexCount() const{
		return GBuffer::getSize() / _layout.getStride();
	}
}