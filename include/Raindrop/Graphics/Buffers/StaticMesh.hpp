#ifndef __RAINDROP_GRAPHICS_BUFFERS_STATIC_MESH_HPP__
#define __RAINDROP_GRAPHICS_BUFFERS_STATIC_MESH_HPP__

#include <Raindrop/Graphics/Buffers/common.hpp>
#include <Raindrop/Graphics/Buffers/Mesh.hpp>

namespace Raindrop::Graphics::Buffers{
	class StaticMesh : public Mesh{
		public:
			StaticMesh(Context& context, const HostMesh& mesh);
			virtual ~StaticMesh() override;
			
			virtual bool hasVertexBuffer() const override;
			virtual bool hasIndexBuffer() const override;

			virtual const Buffer& vertexBuffer() const override;
			virtual const Buffer& indexBuffer() const override;
		
		private:
			Context& _context;

			std::unique_ptr<Buffer> _vbo;
			std::unique_ptr<Buffer> _ibo;

			void createVBO(const std::vector<Vertex>& vertices);
			void createIBO(const std::vector<uint32_t>& indices);
	};
}

#endif