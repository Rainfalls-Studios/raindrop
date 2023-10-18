#include <Raindrop/Graphics/Buffers/HostMesh.hpp>
#include <Raindrop/Graphics/Buffers/Context.hpp>
#include <Raindrop/Graphics/Buffers/VertexLayout.hpp>
#include <Raindrop/Graphics/Buffers/Vertex.hpp>

namespace Raindrop::Graphics::Buffers{
	HostMesh::HostMesh(Context& context, const VertexLayout& layout) : _context{context}{
		_vertexLayout = std::make_unique<VertexLayout>(layout);
		
	}
	
	HostMesh::HostMesh(Context& context) : _context{context}{

	}

	HostMesh::~HostMesh(){}

	const VertexLayout& HostMesh::vertexLayout() const{
		return *_vertexLayout;
	}

	void HostMesh::resize(std::size_t size){
		if (!_vertexLayout){
			_context.logger.warn("Cannot resize a host mesh without a valid vertex layout !");
			throw std::runtime_error("Cannot resize a host mesh without a valid vertex layout !");
		}

		for (std::size_t i=0; i<_bindings.size(); i++){
			_bindings[i].data.resize(size * _vertexLayout->bindingSize(i));
		}
	}

	void* HostMesh::data(std::size_t binding){
		return _bindings[binding].data.data();
	}

	std::size_t HostMesh::bindings() const{
		return _bindings.size();
	}

	std::size_t HostMesh::size() const{
		return _vertexCount * _vertexLayout->size();
	}

	std::size_t HostMesh::vertexCount() const{
		return _vertexCount;
	}
	
	Vertex HostMesh::get(std::size_t vertex){
		return cget(vertex);
	}
	
	const Vertex HostMesh::cget(std::size_t vertex) const{
		return get(vertex);
	}

	const Vertex HostMesh::get(std::size_t vertex) const{
		std::vector<void*> data(_vertexLayout->attributeCount());
		auto attributes = _vertexLayout->attributesNames();
		
		for (auto& attribute : attributes){
			std::size_t id = _vertexLayout->id(attribute);
			std::size_t binding = _vertexLayout->attributeBinding(attribute);
			std::size_t offset = _vertexLayout->attributeOffset(attribute);
			std::size_t size = _vertexLayout->bindingSize(binding);

			data[id] = const_cast<std::uint8_t*>(_bindings[binding].data.data() + (size * vertex) + offset);
		}
		
		return Vertex(_context, data);
	}
}