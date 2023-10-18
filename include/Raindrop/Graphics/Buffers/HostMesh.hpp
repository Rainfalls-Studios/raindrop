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

			template<typename T> T& get(const std::string& attribute, std::size_t vertex);
			template<typename T> const T& get(const std::string& attribute, std::size_t vertex) const;

			void resize(std::size_t size);
			void* data(std::size_t binding);

			std::size_t bindings() const;
			std::size_t vertexCount() const;
			std::size_t size() const;

			Vertex get(std::size_t vertex);
			const Vertex get(std::size_t vertex) const;

		private:
			Context& _context;
			std::unique_ptr<VertexLayout> _vertexLayout;

			struct Binding{
				std::vector<uint8_t> data;
			};

			std::vector<Binding> _bindings;
			std::size_t _vertexCount;
			
			void* get(const std::string& attribute, std::size_t vertex);
			const void* get(const std::string& attribute, std::size_t vertex) const;
			const Vertex cget(std::size_t vertex) const;
	};

	#include <Raindrop/Graphics/Buffers/tpp/HostMesh.tpp>
}

#endif