#include <Raindrop/Graphics/Buffers/Vertex.hpp>
#include <Raindrop/Graphics/Buffers/VertexLayout.hpp>
#include <Raindrop/Graphics/Buffers/Context.hpp>

namespace Raindrop::Graphics::Buffers{
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
}