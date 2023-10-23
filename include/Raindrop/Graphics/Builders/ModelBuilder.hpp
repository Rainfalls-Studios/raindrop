#ifndef __RAINDROP_GRAPHICS_BUILDERS_MODEL_BUILDER_HPP__
#define __RAINDROP_GRAPHICS_BUILDERS_MODEL_BUILDER_HPP__

#include <Raindrop/Graphics/common.hpp>

namespace Raindrop::Graphics::Builders{
	class ModelBuilder{
		public:
			struct MeshData{
				std::vector<Vertex> vertices;
				std::vector<uint32_t> indices;
				bool dynamic;

				MeshData();
			};

			ModelBuilder();
			~ModelBuilder();

			void loadFile(const std::filesystem::path& path);

			void setMeshCount(uint32_t count);

			MeshData& data(uint32_t mesh);
			std::vector<Vertex>& vertices(uint32_t mesh);
			std::vector<uint32_t>& indices(uint32_t mesh);
			
			std::shared_ptr<Model> build(GraphicsContext& context);

			uint32_t meshCount() const;
			const MeshData& data(uint32_t mesh) const;
			const std::vector<Vertex>& vertices(uint32_t mesh) const;
			const std::vector<uint32_t>& indices(uint32_t mesh) const;

		private:
			std::vector<MeshData> _meshes;
	};
}

#endif