#ifndef __RAINDROP_GRAPHICS_BUFFERS_HOST_MESH_HPP__
#define __RAINDROP_GRAPHICS_BUFFERS_HOST_MESH_HPP__

#include <Raindrop/Graphics/Buffers/common.hpp>

namespace Raindrop::Graphics::Buffers{
	class HostMesh{
		public:
			HostMesh(Context& context, const VertexLayout& layout);
			HostMesh(Context& context);
			~HostMesh();
			
			const VertexLayout& vertexLayout() const;

			template<typename T> T& get(const std::string& attribute, std::size_t index);
			template<typename T> const T& get(const std::string& attribute, std::size_t index) const;

			Vertex get(std::size_t index);
			const Vertex get(std::size_t index) const;

			void resize(std::size_t size);
			const void* data(std::size_t binding) const;

			std::size_t bindings() const;
			std::size_t bindingSize(std::size_t binding) const;
			std::size_t vertexCount() const;
			std::size_t size() const;

			std::size_t indexSize() const;
			VkIndexType indexType() const;
			std::size_t indexCount() const;

			bool hashIndices() const;
			const void* indices() const;

			void mergeVerticies();

		private:
			Context& _context;
			std::unique_ptr<VertexLayout> _vertexLayout;

			struct Binding{
				std::vector<std::uint8_t> data;
			};

			std::vector<Binding> _bindings;
			std::size_t _vertexCount;

			std::vector<std::uint8_t> _indices;
			std::size_t _indexSize;
			
			void* get(const std::string& attribute, std::size_t vertex);
			const void* get(const std::string& attribute, std::size_t vertex) const;
			const Vertex cget(std::size_t vertex) const;
	};

	#include <Raindrop/Graphics/Buffers/tpp/HostMesh.tpp>
}

#endif