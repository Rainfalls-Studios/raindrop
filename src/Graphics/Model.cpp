#include <Raindrop/Graphics/Model.hpp>
#include <Raindrop/Graphics/Buffer.hpp>
#include <Raindrop/Graphics/Device.hpp>
#include <Raindrop/Graphics/Mesh/StaticMesh.hpp>
#include <Raindrop/Graphics/GraphicsContext.hpp>
#include <Raindrop/Graphics/FrameState.hpp>

namespace Raindrop::Graphics{
	Model::Model(GraphicsContext& context, const Builders::ModelBuilder& builder) : _context{context}{
		_meshes.resize(builder.meshCount());
		
		for (uint32_t i=0; i<_meshes.size(); i++){
			auto& mesh = _meshes[i];
			const auto& data = builder.data(i);

			if (data.dynamic){
				// mesh = std::make_unique<Mesh::DynamicMesh>()
			} else {
				mesh = std::make_unique<Mesh::StaticMesh>(_context, data.vertices, data.indices);
			}
		}
	}

	Model::~Model(){
		_meshes.clear();
	}
	
	void Model::draw(const FrameState& frameState, VkBuffer* additionalVertexBuffers, uint32_t vertexBufferCount){

		for (const auto& mesh : _meshes){

			if (!mesh->hasVertexBuffer()) continue;

			VkDeviceSize offset = 0;
			VkBuffer vertexBuffer = mesh->vertexBuffer().get();

			vkCmdBindVertexBuffers(
				frameState.commandBuffer,
				0,
				1,
				&vertexBuffer,
				&offset
			);
			
			if (mesh->hasIndexBuffer()){
				vkCmdBindIndexBuffer(
					frameState.commandBuffer,
					mesh->indexBuffer().get(),
					0,
					VK_INDEX_TYPE_UINT32
				);

				vkCmdDrawIndexed(
					frameState.commandBuffer,
					mesh->indexBuffer().instanceCount(),
					1,
					0,
					0,
					0
				);
			} else {
				vkCmdDraw(
					frameState.commandBuffer,
					mesh->indexBuffer().instanceCount(),
					1,
					0,
					0
				);
			}
		}
	}
}
