
#ifndef __RAINDROP_GRAPHICS_MESH_HPP__
#define __RAINDROP_GRAPHICS_MESH_HPP__

#include "types.hpp"
#include "pch.pch"
#include "VertexLayout.hpp"
#include <Raindrop/Object.hpp>

namespace Raindrop::Graphics{
	class Mesh : public Object{
		public:
			static std::shared_ptr<Mesh> create(Raindrop::Context& context);

			Mesh() noexcept;
			~Mesh();

			Mesh(const Mesh&) = delete;
			Mesh& operator=(const Mesh&) = delete;

			Mesh(Mesh&& other);
			Mesh& operator=(Mesh&& other);

			friend void swap(Mesh& A, Mesh& B);

			Mesh& prepare(Context& context);
			void initialize();
			void release();

			Mesh& setData(const MeshData& data);

			const VkIndexType& getIndexType() const noexcept;
			bool hasIndexBuffer() const noexcept;
			const Buffer* getIndexBuffer() const noexcept;
			const std::vector<Buffer>& getVertexBuffers() const noexcept;

			const VertexLayout& getVertexLayout() const noexcept;
			const std::size_t& getVertexCount() const noexcept;

		private:
			Context* _context;
			const MeshData* _data;
			
			VkIndexType _indexType;
			std::unique_ptr<Buffer> _indexBuffer;
			std::vector<Buffer> _vertexBuffers;
			VertexLayout _layout;
			std::size_t _vertexCount;

			void createIndexBuffer();
			void createVertexBuffers();
	};
}

#endif