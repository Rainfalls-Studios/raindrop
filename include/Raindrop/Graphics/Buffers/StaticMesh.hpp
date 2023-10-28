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

			virtual const Buffer& vertexBuffer(std::size_t index) const override;
			virtual const Buffer& indexBuffer() const override;

			virtual std::size_t vertexBufferCount() const override;
		
		private:
			Context& _context;
			const VertexLayout& _layout;

			struct SubmitInfo;

			std::vector<std::unique_ptr<Buffer>> _vertexBuffers;
			std::unique_ptr<Buffer> _indexBuffer;

			void createVertexBuffers(const HostMesh& mesh, SubmitInfo& submitInfo);
			void createIndexBuffer(const HostMesh& mesh, SubmitInfo& submitInfo);
	};
}

#endif