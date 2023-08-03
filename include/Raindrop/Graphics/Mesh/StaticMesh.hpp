#ifndef __RAINDROP_GRAPHICS_MESH_STATIC_MESH_HPP__
#define __RAINDROP_GRAPHICS_MESH_STATIC_MESH_HPP__

#include <Raindrop/Graphics/Mesh/common.hpp>
#include <Raindrop/Graphics/Mesh/Mesh.hpp>

namespace Raindrop::Graphics::Mesh{
	class StaticMesh : public Mesh{
		public:
			StaticMesh(GraphicsContext& context, const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices = {});
			virtual ~StaticMesh() override;
			
			virtual bool hasVertexBuffer() const override;
			virtual bool hasIndexBuffer() const override;

			virtual const Buffer& vertexBuffer() const override;
			virtual const Buffer& indexBuffer() const override;
		
		private:
			GraphicsContext& _context;

			std::unique_ptr<Buffer> _vbo;
			std::unique_ptr<Buffer> _ibo;

			void createVBO(const std::vector<Vertex>& vertices);
			void createIBO(const std::vector<uint32_t>& indices);
	};
}

#endif