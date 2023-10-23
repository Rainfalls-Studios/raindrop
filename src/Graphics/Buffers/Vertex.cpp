#include <Raindrop/Graphics/Buffers/Vertex.hpp>
#include <Raindrop/Graphics/Buffers/VertexLayout.hpp>
#include <Raindrop/Graphics/Buffers/Context.hpp>

std::size_t hashVoidPointer(const void* data, std::size_t len) {
	const std::size_t seed = 0;
	const unsigned char* buffer = static_cast<const unsigned char*>(data);
	std::size_t hash = seed;

	for (std::size_t i = 0; i < len; ++i) {
		hash ^= buffer[i] + 0x9e3779b9 + (hash << 6) + (hash >> 2);
	}

	return hash;
}

namespace Raindrop::Graphics::Buffers{
	Vertex::Vertex(const VertexLayout& layout) : _layout{std::make_unique<VertexLayout>(layout)}{	
		_reference = false;

		_attributes.resize(layout.attributeCount());
		for (std::size_t i=0; i<_attributes.size(); i++){
			void*& attribute = _attributes[i];
			std::size_t size = layout.attributeSize(i);

			attribute = std::malloc(size);
			if (!attribute){
				_layout->_context.logger.critical("Malloc error !");
				throw std::runtime_error("Malloc error");
			}

			std::memset(attribute, 0, size);
		}
	}

	Vertex::Vertex(const VertexLayout& layout, const std::vector<void*>& attributes) : _layout{std::make_unique<VertexLayout>(layout)}{
		_reference = true;
		_attributes = attributes;
	}

	Vertex::~Vertex(){
		if (_reference) return;
		for (auto& attribute : _attributes){
			if (attribute) std::free(attribute);
		}
	}

	Vertex::Vertex(const Vertex& other) : Vertex(*other._layout){
		for (std::size_t i=0; i<_attributes.size(); i++){
			std::size_t size = _layout->attributeSize(i);
			void* dest = _attributes[i];
			const void* src = other._attributes[i];

			std::memcpy(dest, src, size);
		}
	}

	bool Vertex::operator==(const Vertex& other) const{
		if (*_layout != *other._layout) return false;

		for (std::size_t i=0; i<_attributes.size(); i++){
			std::size_t size = _layout->attributeSize(i);
			const void* a = _attributes[i];
			const void* b = other._attributes[i];

			if (std::memcmp(a, b, size) != 0) return false;
		}
		return true;
	}

	bool Vertex::operator!=(const Vertex& other) const{
		return !(*this == other);
	}

	const VertexLayout& Vertex::layout() const{
		return *_layout;
	}

	std::size_t Vertex::hash(std::size_t seed) const{
		for (std::size_t i=0; i<_attributes.size(); i++){
			const void* attribute = _attributes[i];
			std::size_t size = _layout->attributeSize(i);

			std::size_t hash = hashVoidPointer(attribute, size);
			seed ^= hash + 0x9e3779b9 + (seed << 6) + (seed >> 2);
		}
		return seed;
	}
}