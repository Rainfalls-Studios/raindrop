#include <Raindrop/Graphics/Builders/ModelBuilder.hpp>
#include <Raindrop/Graphics/Model.hpp>

#include <tiny_obj_loader.h>
#include <glm/gtx/hash.hpp>

template <typename T, typename... Rest>
void hashCombine(std::size_t &seed, const T &v, const Rest&... rest){
    seed ^= std::hash<T>{}(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    (hashCombine(seed, rest), ...);
}

namespace std{
	template<>
	struct hash<Raindrop::Graphics::Vertex> {
		size_t operator()(Raindrop::Graphics::Vertex const &vertex) const{
			size_t seed = 0;
			hashCombine(seed, vertex.position, vertex.color, vertex.normal, vertex.uv);
			return seed;
		}
	};
}

namespace Raindrop::Graphics::Builders{
	ModelBuilder::MeshData::MeshData() : 
		dynamic{false}{
	}

	std::shared_ptr<Model> ModelBuilder::build(GraphicsContext& context){
		return std::make_shared<Model>(context, *this);
	}

	ModelBuilder::ModelBuilder(){}
	ModelBuilder::~ModelBuilder(){}

	void ModelBuilder::loadFile(const std::filesystem::path& path){
		tinyobj::attrib_t attrib;
		std::vector<tinyobj::shape_t> shapes;
		std::vector<tinyobj::material_t> materials;
		std::string warn, err;

		// tinyobj::LoadObj()
		if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, path.string().c_str())) {
			throw std::runtime_error(warn + err);
		}

		_meshes.resize(shapes.size());

		for (size_t i=0; i<shapes.size(); i++){
			auto& shape = shapes[i];
			auto& mesh = _meshes[i];

			auto& vertices = mesh.vertices;
			auto& indices = mesh.indices;

			std::unordered_map<Vertex, uint32_t> uniqueVertices;

			for (const auto &index : shape.mesh.indices) {
				Vertex vertex{};

				if (index.vertex_index >= 0) {
					vertex.position = {
						attrib.vertices[3 * index.vertex_index + 0],
						attrib.vertices[3 * index.vertex_index + 1],
						attrib.vertices[3 * index.vertex_index + 2],
					};

					vertex.color = {
						attrib.colors[3 * index.vertex_index + 0],
						attrib.colors[3 * index.vertex_index + 1],
						attrib.colors[3 * index.vertex_index + 2],
					};
				}

				if (index.normal_index >= 0) {
					vertex.normal = {
						attrib.normals[3 * index.normal_index + 0],
						attrib.normals[3 * index.normal_index + 1],
						attrib.normals[3 * index.normal_index + 2],
					};
				}

				if (index.texcoord_index >= 0) {
					vertex.uv = {
						attrib.texcoords[2 * index.texcoord_index + 0],
						attrib.texcoords[2 * index.texcoord_index + 1],
					};
				}

				if (uniqueVertices.count(vertex) == 0){
					uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
					vertices.push_back(vertex);
				}

				indices.push_back(uniqueVertices[vertex]);
			}
		}
	}

	void ModelBuilder::setMeshCount(uint32_t count){
		_meshes.resize(count);
	}

	ModelBuilder::MeshData& ModelBuilder::data(uint32_t mesh){
		return _meshes[mesh];
	}
	
	std::vector<Vertex>& ModelBuilder::vertices(uint32_t mesh){
		return _meshes[mesh].vertices;
	}

	std::vector<uint32_t>& ModelBuilder::indices(uint32_t mesh){
		return _meshes[mesh].indices;
	}
	
	uint32_t ModelBuilder::meshCount() const{
		return _meshes.size();
	}

	const ModelBuilder::MeshData& ModelBuilder::data(uint32_t mesh) const{
		return _meshes[mesh];
	}

	const std::vector<Vertex>& ModelBuilder::vertices(uint32_t mesh) const{
		return _meshes[mesh].vertices;
	}

	const std::vector<uint32_t>& ModelBuilder::indices(uint32_t mesh) const{
		return _meshes[mesh].indices;
	}
}