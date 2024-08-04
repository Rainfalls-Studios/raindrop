
#ifndef __RAINDROP_GRAPHICS_MESH_HPP__
#define __RAINDROP_GRAPHICS_MESH_HPP__

#include "types.hpp"
#include "pch.pch"

namespace Raindrop::Graphics{
	class Mesh{
		public:
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

		private:
			Context* _context;
			const MeshData* _data;
			
			VkIndexType _indexType;
			std::unique_ptr<Buffer> _indexBuffer;
			std::vector<Buffer> _vertexBuffers;

			void createIndexBuffer();
			void createVertexBuffers();
	};
}

#endif