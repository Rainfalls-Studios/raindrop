#include <Raindrop/Graphics/Buffers/HostMesh.hpp>
#include <Raindrop/Graphics/Buffers/Context.hpp>
#include <Raindrop/Graphics/Buffers/VertexLayout.hpp>
#include <Raindrop/Graphics/Buffers/Vertex.hpp>

namespace Raindrop::Graphics::Buffers{
	HostMesh::HostMesh(Context& context, const VertexLayout& layout) : _context{context}{
		_vertexLayout = std::make_unique<VertexLayout>(layout);
	}
	
	HostMesh::HostMesh(Context& context) : _context{context}{}
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

	const void* HostMesh::data(std::size_t binding) const{
		return _bindings[binding].data.data();
	}

	std::size_t HostMesh::bindings() const{
		return _bindings.size();
	}
	
	std::size_t HostMesh::bindingSize(std::size_t binding) const{
		return _vertexLayout->bindingSize(binding);
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
		
		for (std::size_t i=0; i<data.size(); i++){
			std::size_t binding = _vertexLayout->attributeBinding(i);
			std::size_t offset = _vertexLayout->attributeOffset(i);
			std::size_t size = _vertexLayout->bindingSize(binding);

			data[i] = const_cast<std::uint8_t*>(_bindings[binding].data.data() + (size * vertex) + offset);
		}
		
		return Vertex(_context, data);
	}

	
	std::size_t HostMesh::indexSize() const{
		return _indexSize;
	}

	VkIndexType HostMesh::indexType() const{
		switch (_indexSize){
			case 1: return VK_INDEX_TYPE_UINT8_EXT;
			case 2: return VK_INDEX_TYPE_UINT16;
			case 4: return VK_INDEX_TYPE_UINT32;
		}
		return VK_INDEX_TYPE_NONE_KHR;
	}

	bool HostMesh::hashIndices() const{
		return _indexSize == 0;
	}

	std::size_t HostMesh::indexCount() const{
		return _indices.size();
	}

	const void* HostMesh::indices() const{
		return static_cast<const void*>(_indices.data());
	}

	void HostMesh::mergeVerticies(){
		// TODO :: add uint8_t and uint16_t support
		_indexSize = sizeof(uint32_t);
		std::unordered_map<Vertex, uint32_t> uniqueVertices;

		std::size_t uniqueVertexCount = 0;
		std::vector<uint32_t> indices;
		std::vector<Binding> uniqueBindings(_bindings.size());

		for (std::size_t i=0; i<_bindings.size(); i++){
			uniqueBindings[i].data.resize(_vertexCount * _vertexLayout->bindingSize(i));
		}

		for (std::size_t i=0; i<_vertexCount; i++){
			Vertex vertex = get(i);

			if (uniqueVertices.count(vertex) == 0){
				uniqueVertices[vertex] = uniqueVertexCount;
				uniqueVertexCount++;

				for (std::size_t binding = 0; binding < _bindings.size(); binding++){
					std::size_t size = _vertexLayout->bindingSize(binding);
					void* dest = uniqueBindings[binding].data[i * size];
					const void* src = _bindings[binding].data[i * size];

					std::memcpy(dest, src, size)
				}
			}

			indices.push_back(uniqueVertices[vertex]);
		}

		_bindings = uniqueBindings;
		_indices.resize(_indexSize * indices.size());

		std::memcpy(_indices.data(), indices.data(), _indexSize * indices.size());
	}
}