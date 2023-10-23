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
	Vertex::Vertex() : _layout{nullptr}{}
	

	Vertex::Vertex(const VertexLayout& layout) :
		_layout{std::make_unique<VertexLayout>(layout)},
		_reference{false}{
		_attributes.resize(_layout->attributeCount());

		auto names = _layout->attributesNames();

		for (const auto& name : names){
			std::size_t id = _layout->id(name);
			std::size_t size = _layout->attributeSize(name);

			_attributes[id] = std::malloc(size);
			if (!_attributes[id]) throw std::system_error();

			std::memset(_attributes[id], 0, size);
		}
	}

	Vertex::Vertex(const VertexLayout& layout, const std::vector<void*>& attributes) :
		_layout{std::make_unique<VertexLayout>(layout)},
		_attributes{attributes},
		_reference{true}{}

	Vertex::~Vertex(){
		if (!_reference){
			for (auto& attribute : _attributes){
				std::free(attribute);
			}
		}
		_layout.reset();
	}

	const VertexLayout& Vertex::layout() const{
		return *_layout;
	}

	void* Vertex::get(const std::string& attribute){
		return _attributes[_layout->id(attribute)];
	}

	const void* Vertex::get(const std::string& attribute) const{
		return _attributes[_layout->id(attribute)];
	}

	bool Vertex::operator==(const Vertex& other) const{
		if (other._layout != _layout) return false;

		for (std::size_t i=0; i<_attributes.size(); i++){
			const void* a = _attributes[i];
			const void* b = other._attributes[i];

			const std::string& name = _layout->name(i);
			std::size_t size = _layout->attributeSize(name);

			if (std::memcmp(a, b, size) != 0) return false;
		}
		return true;
	}

	std::size_t Vertex::hash(std::size_t seed) const{
		for (std::size_t i=0; i<_attributes.size(); i++){
			const void* attribute = _attributes[i];
			const std::string& name = _layout->name(i);
			std::size_t size = _layout->attributeSize(name);

			std::size_t hash = hashVoidPointer(attribute, size);
			seed ^= hash + 0x9e3779b9 + (seed << 6) + (seed >> 2);
		}
		return seed;
	}
}