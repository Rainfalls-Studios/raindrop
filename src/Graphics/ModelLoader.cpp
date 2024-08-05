#include <Raindrop/Graphics/ModelLoader.hpp>
#include <Raindrop/Graphics/Context.hpp>
#include <Raindrop/Graphics/Model.hpp>
#include <Raindrop/Graphics/Mesh.hpp>
#include <Raindrop/Graphics/MeshData.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace Raindrop::Graphics{
	ModelLoader::ModelLoader() noexcept :	
		_context{nullptr}
	{}

	ModelLoader::~ModelLoader(){
		release();
	}

	ModelLoader& ModelLoader::prepare(Context& context){
		_context = &context;

		return *this;
	}

	void ModelLoader::initialize(){

	}

	void ModelLoader::release(){
		_context = nullptr;
	}

	std::shared_ptr<Model> ModelLoader::load(const std::filesystem::path& filepath, const std::uint32_t& properties){
		if (!_context){
			spdlog::warn("Attempt to load a model using a non prepared model loader");
			throw std::runtime_error("The model loader has not been prepared");
		}

		_context->logger->info("Loading model \"{}\"...", filepath.string());
		auto meshesData = loadMeshes(filepath, properties);

		std::vector<Mesh> meshes(meshesData.size());
		for (std::size_t i=0; i<meshes.size(); i++){
			const auto& data = meshesData[i];
			auto& mesh = meshes[i];

			mesh.prepare(*_context)
				.setData(data)
				.initialize();
		}

		std::shared_ptr<Model> model = std::make_shared<Model>();
		model->prepare(*_context)
			.setMeshes(std::move(meshes))
			.initialize();
		
		return model;
	}

	std::vector<MeshData> ModelLoader::loadMeshes(const std::filesystem::path& filepath, const std::uint32_t& properties){
		if (!_context){
			spdlog::warn("Attempt to load a model using a non prepared model loader");
			throw std::runtime_error("The model loader has not been prepared");
		}


		_context->logger->info("Loading meshes \"{}\"...", filepath.string());
		Assimp::Importer importer;

		uint flags = aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_SortByPType | aiProcess_OptimizeMeshes;

		if (properties & NORMAL) flags |= aiProcess_GenNormals;
		if (properties & SMOOTH_NORMAL) flags |= aiProcess_GenSmoothNormals;
		if (properties & UV) flags |= aiProcess_GenUVCoords | aiProcess_FlipUVs;
		if (properties & TANGENT) flags |= aiProcess_CalcTangentSpace;
		
		_context->logger->trace("Reading model file \"{}\"...", filepath.string());
		const aiScene* scene = importer.ReadFile(filepath.string(), flags);
		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode){
			_context->logger->error("Failed to load model scene \"{}\" : reason  : {}", filepath.string(), importer.GetErrorString());
			throw std::runtime_error("Failed to load model scene");
		}

		std::vector<MeshData> meshes(scene->mNumMeshes);
		_context->logger->trace("Model file loaded successfully \"{}\", found {} meshes", filepath.string(), scene->mNumMeshes);

		VertexLayout layout = createVertexLayout(properties);

		for (std::size_t i=0; i<scene->mNumMeshes; i++){
			const auto& data = scene->mMeshes[i];
			auto& mesh = meshes[i];

			switch (data->mPrimitiveTypes){
				case aiPrimitiveType_LINE: mesh.setPrimitiveTopology(VK_PRIMITIVE_TOPOLOGY_LINE_LIST); break;
				case aiPrimitiveType_POINT: mesh.setPrimitiveTopology(VK_PRIMITIVE_TOPOLOGY_POINT_LIST); break;
				case aiPrimitiveType_TRIANGLE: mesh.setPrimitiveTopology(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST); break;
				default: {
					_context->logger->warn("Invalid or unssuported primitive type while loading model : \"{}\" | primitive : {}", filepath.string(), data->mPrimitiveTypes);
					throw std::runtime_error("The primitive type is invalid or unsupported");
				}
			}

			mesh.prepare(*_context)
				.setLayout(layout)
				.allocate(data->mNumVertices);

			auto binding = mesh.getBinding("binding 0");

			if (properties & POSITION){
				auto positions = binding.get<glm::vec3>("position");

				for (std::size_t n=0; n<data->mNumVertices; n++){
					auto& position = data->mVertices[n];
					positions[n] = glm::vec3(position.x, position.y, position.z);
				}
			}

			if (properties & NORMAL){
				auto normals = binding.get<glm::vec3>("normal");

				if (data->mNormals == nullptr){
					_context->logger->warn("The model \"{}\" does not contains normals as required", filepath.string());
					for (std::size_t n=0; n<data->mNumVertices; n++){
						normals[n] = glm::vec3(1.f, 0.f, 0.f);
					}
				} else {
					for (std::size_t n=0; n<data->mNumVertices; n++){
						const auto& normal = data->mNormals[n];
						normals[n] = glm::vec3(normal.x, normal.y, normal.z);
					}
				}
			}

			if (properties & COLOR){
				auto colors = binding.get<glm::vec3>("color");

				if (data->mColors[0] == nullptr){
					_context->logger->warn("The model \"{}\" does not contains colors as required", filepath.string());
					for (std::size_t n=0; n<data->mNumVertices; n++){
						colors[n] = glm::vec3(1.f);
					}
				} else {
					for (std::size_t n=0; n<data->mNumVertices; n++){
						const auto& color = data->mColors[0][n];
						colors[n] = glm::vec3(color.r, color.g, color.b);
					}
				}
			}
			
			if (properties & UV){
				auto UVs = binding.get<glm::vec2>("UV");

				if (data->mTextureCoords[0] == nullptr){
					_context->logger->warn("The model \"{}\" does not contains UVs as required", filepath.string());
					for (std::size_t n=0; n<data->mNumVertices; n++){
						UVs[n] = glm::vec2(0.f);
					}
				} else {
					for (std::size_t n=0; n<data->mNumVertices; n++){
						const auto& UV = data->mTextureCoords[0][n];
						UVs[n] = glm::vec2(UV.x, UV.y);
					}
				}
			}

			if (properties & WEIGHT){
				_context->logger->warn("NOT SUPPORTED !");
				auto weights = binding.get<uint32_t>("weight");

				for (std::size_t n=0; n<data->mNumVertices; n++){
					weights[n] = 0;
				}
			}

			if (properties & TANGENT){
				auto tangents = binding.get<glm::vec3>("tangent");

				if (data->mTangents == nullptr){
					_context->logger->warn("The model \"{}\" does not contains tangents as required", filepath.string());
					for (std::size_t n=0; n<data->mNumVertices; n++){
						tangents[n] = glm::vec3(1.f);
					}
				} else {
					for (std::size_t n=0; n<data->mNumVertices; n++){
						const auto& tangent = data->mTangents[n];
						tangents[n] = glm::vec3(tangent.x, tangent.y, tangent.z);
					}
				}
			}

			if (properties & BITANGENT){
				auto bitangents = binding.get<glm::vec3>("bitangent");

				if (data->mBitangents == nullptr){
					_context->logger->warn("The model \"{}\" does not contains bitangents as required", filepath.string());
					for (std::size_t n=0; n<data->mNumVertices; n++){
						bitangents[n] = glm::vec3(1.f);
					}
				} else {
					for (std::size_t n=0; n<data->mNumVertices; n++){
						const auto& bitangent = data->mBitangents[n];
						bitangents[n] = glm::vec3(bitangent.x, bitangent.y, bitangent.z);
					}
				}
			}


			if (data->mNumFaces){
				for (std::size_t n=0; n<data->mNumFaces; n++){
					const aiFace& face = data->mFaces[n];

					for (std::size_t j=0; j<face.mNumIndices; j++){
						mesh.pushIndex(face.mIndices[j]);
					}
				}
			}
		}

		_context->logger->trace("Meshes loaded successfully !");
		return std::move(meshes);
	}

	VertexLayout ModelLoader::createVertexLayout(std::uint32_t properties){
		VertexLayout layout;

		auto binding = layout.addBinding("binding 0");

		if (properties & POSITION) layout.addAttribute<glm::vec3>(binding, "position");
		if (properties & (NORMAL | SMOOTH_NORMAL)) layout.addAttribute<glm::vec3>(binding, "normal");
		if (properties & COLOR) layout.addAttribute<glm::vec3>(binding, "color");
		if (properties & UV) layout.addAttribute<glm::vec2>(binding, "UV");
		if (properties & WEIGHT) layout.addAttribute<uint32_t>(binding, "weight");
		if (properties & TANGENT) layout.addAttribute<glm::vec3>(binding, "tangent");
		if (properties & BITANGENT) layout.addAttribute<glm::vec3>(binding, "bitangent");

		return layout;
	}
}