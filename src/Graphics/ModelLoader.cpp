#include <Raindrop/Graphics/ModelLoader.hpp>
#include <Raindrop/Graphics/Context.hpp>
#include <Raindrop/Graphics/Model.hpp>
#include <Raindrop/Graphics/Mesh.hpp>
#include <Raindrop/Graphics/MeshData.hpp>
#include <Raindrop/Graphics/ModelLayoutConfig.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#define copy_component_double(component, componentFnc) if (component.size != 0) *(reinterpret_cast<double*>(dest) + (component.offset / BIT_PER_DOUBLE)) = componentFnc(input, component.type);
#define copy_component_float(component, componentFnc) if (component.size != 0) *(reinterpret_cast<float*>(dest) + (component.offset / BIT_PER_FLOAT)) = componentFnc(input, component.type);
#define set_component_double_null(component) if (component.size != 0)  *(reinterpret_cast<double*>(dest) + (component.offset / BIT_PER_DOUBLE)) = 0.d;
#define set_component_float_null(component) if (component.size != 0)  *(reinterpret_cast<float*>(dest) + (component.offset / BIT_PER_FLOAT)) = 0.f;
#define get_component_info(component, type) Formats::ComponentInfo component = getComponentInfo(componentSwizzle, formatInfo, Formats::ComponentType::type);


namespace Raindrop::Graphics{
	static constexpr std::uint32_t BIT_PER_DOUBLE = sizeof(double) * 8;
	static constexpr std::uint32_t BIT_PER_FLOAT = sizeof(float) * 8;
	static constexpr std::uint32_t INVALID_COMPONENT_INDEX = UINT32_MAX;


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

	std::shared_ptr<Model> ModelLoader::load(const std::filesystem::path& filepath, const ModelLayoutConfig& config){
		if (!_context){
			spdlog::warn("Attempt to load a model using a non prepared model loader");
			throw std::runtime_error("The model loader has not been prepared");
		}

		_context->logger->info("Loading model \"{}\"...", filepath.string());
		auto meshesData = loadMeshes(filepath, config);

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

	Formats::ComponentInfo getComponentInfo(const Formats::ComponentSwizzle& componentSwizzle, const Formats::FormatInfo& info, const Formats::ComponentType& component) noexcept{
		Formats::ComponentType swizzle = componentSwizzle.get(component);
		const Formats::ComponentInfo* componentInfo = info.getComponent(swizzle);

		// The format does not support the component
		if (!componentInfo){
			return Formats::ComponentInfo{
				.type = Formats::ComponentType::NONE,
				.size = 0,
				.offset = UINT32_MAX
			};
		}

		return *componentInfo;
	}

	std::uint32_t formatComponentTypeToVertexComponentIndex(const Formats::ComponentType& type){
		switch (type){
			case Formats::ComponentType::R: return 0;
			case Formats::ComponentType::G: return 1;
			case Formats::ComponentType::B: return 2;
			case Formats::ComponentType::A: return 3;
			default: return INVALID_COMPONENT_INDEX;
		}
	}

	const ai_real& getAiVec3Component(const aiVector3D& vec, const Formats::ComponentType& component){
		std::uint32_t index = formatComponentTypeToVertexComponentIndex(component);
		static const float DEFAULT = 0.f;

		switch (index){
			case 0: return vec.x;
			case 1: return vec.y;
			case 2: return vec.z;
			default: return DEFAULT;
		}
	}

	const ai_real& getAiVec2Component(const aiVector2D& vec, const Formats::ComponentType& component){
		std::uint32_t index = formatComponentTypeToVertexComponentIndex(component);
		static const float DEFAULT = 0.f;

		switch (index){
			case 0: return vec.x;
			case 1: return vec.y;
			default: return DEFAULT;
		}
	}

	const ai_real& getAiColor3Component(const aiColor3D& vec, const Formats::ComponentType& component){
		std::uint32_t index = formatComponentTypeToVertexComponentIndex(component);
		static const float DEFAULT = 0.f;

		switch (index){
			case 0: return vec.r;
			case 1: return vec.g;
			case 2: return vec.b;
			default: return DEFAULT;
		}
	}

	const ai_real& getAiColor4Component(const aiColor4D& vec, const Formats::ComponentType& component){
		std::uint32_t index = formatComponentTypeToVertexComponentIndex(component);
		static const float DEFAULT = 0.f;

		switch (index){
			case 0: return vec.r;
			case 1: return vec.g;
			case 2: return vec.b;
			case 3: return vec.a;
			default: return DEFAULT;
		}
	}

	VkPrimitiveTopology getPrimitiveTopology(const std::uint32_t& flags){
		switch (flags){
			case aiPrimitiveType_LINE: return VK_PRIMITIVE_TOPOLOGY_LINE_LIST;
			case aiPrimitiveType_POINT: return VK_PRIMITIVE_TOPOLOGY_POINT_LIST;
			case aiPrimitiveType_TRIANGLE: return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
			default: {
				throw std::runtime_error("The primitive type is invalid or unsupported");
			}
		}
	}

	VertexLayout::UsageFlags getMeshLayoutUsage(const aiMesh* mesh){
		VertexLayout::UsageFlags usage = 0;

		if (mesh->HasPositions()) usage |= VertexLayout::POSITION;
		if (mesh->HasNormals()) usage |= VertexLayout::NORMAL;
		if (mesh->HasVertexColors(0)) usage |= VertexLayout::COLOR;
		if (mesh->HasTangentsAndBitangents()) usage |= (VertexLayout::TANGENT | VertexLayout::BITANGENT);
		if (mesh->HasTextureCoords(0)) usage |= VertexLayout::UV;

		return usage;
	}

	struct AttributeLoadData{
		const VertexLayout& layout;
		const std::filesystem::path& filepath;
		Context& context;
		MeshData& mesh;
		const aiMesh* data;
	};

	void loadPositions(AttributeLoadData& loadData){
		const VertexLayout& layout = loadData.layout;
		const std::filesystem::path& filepath = loadData.filepath;
		Context& context = loadData.context;
		MeshData& mesh = loadData.mesh;
		const aiMesh* data = loadData.data;

		auto positionAttributes = layout.getDedicatedAttributes(VertexLayout::POSITION);

		for (const auto& attribute : positionAttributes){
			auto positions = mesh.getBinding(attribute.binding).get(static_cast<std::size_t>(attribute.location));

			const Formats::FormatInfo formatInfo = Formats::getInfo(attribute.format);
			const auto& componentSwizzle = attribute.componentMapping;

			// get the components info
			get_component_info(r, RED);
			get_component_info(g, GREEN);
			get_component_info(b, BLUE);
			get_component_info(a, ALPHA);

			// Note that, since these are vertex attributes cannot be smaller than 1 bytes (but can be decomposed),
			// there is no need to handle bit per bit copy

			// For now, the vertex usage are only in float formats, so no need to support integers for now
			// TODO: integers support

			const bool isFormatDouble = r.size == BIT_PER_DOUBLE;
			if (isFormatDouble){
				for (std::size_t n=0; n<data->mNumVertices; n++){
					const auto& input = data->mVertices[n];
					void* dest = positions.getRaw(n);

					copy_component_double(r, getAiVec3Component);
					copy_component_double(g, getAiVec3Component);
					copy_component_double(b, getAiVec3Component);
					copy_component_double(a, getAiVec3Component);
				}
			} else {
				for (std::size_t n=0; n<data->mNumVertices; n++){
					const auto& input = data->mVertices[n];
					void* dest = positions.getRaw(n);

					copy_component_float(r, getAiVec3Component);
					copy_component_float(g, getAiVec3Component);
					copy_component_float(b, getAiVec3Component);
					copy_component_float(a, getAiVec3Component);
				}
			}
		}
	}

	void loadUVs(AttributeLoadData& loadData){
		const VertexLayout& layout = loadData.layout;
		const std::filesystem::path& filepath = loadData.filepath;
		Context& context = loadData.context;
		MeshData& mesh = loadData.mesh;
		const aiMesh* data = loadData.data;

		auto positionAttributes = layout.getDedicatedAttributes(VertexLayout::UV);

		for (const auto& attribute : positionAttributes){
			auto uvs = mesh.getBinding(attribute.binding).get(static_cast<std::size_t>(attribute.location));

			const Formats::FormatInfo formatInfo = Formats::getInfo(attribute.format);
			const auto& componentSwizzle = attribute.componentMapping;

			// get the components info
			get_component_info(r, RED);
			get_component_info(g, GREEN);
			get_component_info(b, BLUE);
			get_component_info(a, ALPHA);

			// Note that, since these are vertex attributes cannot be smaller than 1 bytes (but can be decomposed),
			// there is no need to handle bit per bit copy

			// For now, the vertex usage are only in float formats, so no need to support integers for now
			// TODO: integers support

			const bool isFormatDouble = r.size == BIT_PER_DOUBLE;
			if (isFormatDouble){
				for (std::size_t n=0; n<data->mNumVertices; n++){
					const auto& input = data->mTextureCoords[0][n];
					void* dest = uvs.getRaw(n);

					copy_component_double(r, getAiVec3Component);
					copy_component_double(g, getAiVec3Component);
					copy_component_double(b, getAiVec3Component);
					copy_component_double(a, getAiVec3Component);
				}
			} else {
				for (std::size_t n=0; n<data->mNumVertices; n++){
					const auto& input = data->mTextureCoords[0][n];
					void* dest = uvs.getRaw(n);

					copy_component_float(r, getAiVec3Component);
					copy_component_float(g, getAiVec3Component);
					copy_component_float(b, getAiVec3Component);
					copy_component_float(a, getAiVec3Component);
				}
			}
		}
	}

	void loadColors(AttributeLoadData& loadData){
		const VertexLayout& layout = loadData.layout;
		const std::filesystem::path& filepath = loadData.filepath;
		Context& context = loadData.context;
		MeshData& mesh = loadData.mesh;
		const aiMesh* data = loadData.data;

		auto positionAttributes = layout.getDedicatedAttributes(VertexLayout::COLOR);

		for (const auto& attribute : positionAttributes){
			auto colors = mesh.getBinding(attribute.binding).get(static_cast<std::size_t>(attribute.location));

			const Formats::FormatInfo formatInfo = Formats::getInfo(attribute.format);
			const auto& componentSwizzle = attribute.componentMapping;

			// get the components info
			get_component_info(r, RED);
			get_component_info(g, GREEN);
			get_component_info(b, BLUE);
			get_component_info(a, ALPHA);

			// Note that, since these are vertex attributes cannot be smaller than 1 bytes (but can be decomposed),
			// there is no need to handle bit per bit copy

			// For now, the vertex usage are only in float formats, so no need to support integers for now
			// TODO: integers support

			const bool isFormatDouble = r.size == BIT_PER_DOUBLE;
			if (data->mColors[0]){
				if (isFormatDouble){
					for (std::size_t n=0; n<data->mNumVertices; n++){
						const auto& input = data->mColors[0][n];
						void* dest = colors.getRaw(n);

						copy_component_double(r, getAiColor4Component);
						copy_component_double(g, getAiColor4Component);
						copy_component_double(b, getAiColor4Component);
						copy_component_double(a, getAiColor4Component);
					}
				} else {
					for (std::size_t n=0; n<data->mNumVertices; n++){
						const auto& input = data->mColors[0][n];
						void* dest = colors.getRaw(n);

						copy_component_float(r, getAiColor4Component);
						copy_component_float(g, getAiColor4Component);
						copy_component_float(b, getAiColor4Component);
						copy_component_float(a, getAiColor4Component);
					}
				}
			} else {
				context.logger->warn("The mesh does not contain colors as required");
				if (isFormatDouble){
					for (std::size_t n=0; n<data->mNumVertices; n++){
						void* dest = colors.getRaw(n);
						
						set_component_double_null(r);
						set_component_double_null(g);
						set_component_double_null(b);
						set_component_double_null(a);
					}
				} else {
					for (std::size_t n=0; n<data->mNumVertices; n++){
						void* dest = colors.getRaw(n);
						
						set_component_float_null(r);
						set_component_float_null(g);
						set_component_float_null(b);
						set_component_float_null(a);
					}
				}
			}
		}
	}

	void loadNormals(AttributeLoadData& loadData){
		const VertexLayout& layout = loadData.layout;
		const std::filesystem::path& filepath = loadData.filepath;
		Context& context = loadData.context;
		MeshData& mesh = loadData.mesh;
		const aiMesh* data = loadData.data;

		auto positionAttributes = layout.getDedicatedAttributes(VertexLayout::NORMAL);

		for (const auto& attribute : positionAttributes){
			auto normals = mesh.getBinding(attribute.binding).get(static_cast<std::size_t>(attribute.location));

			const Formats::FormatInfo formatInfo = Formats::getInfo(attribute.format);
			const auto& componentSwizzle = attribute.componentMapping;

			// get the components info
			get_component_info(r, RED);
			get_component_info(g, GREEN);
			get_component_info(b, BLUE);
			get_component_info(a, ALPHA);

			// Note that, since these are vertex attributes cannot be smaller than 1 bytes (but can be decomposed),
			// there is no need to handle bit per bit copy

			// For now, the vertex usage are only in float formats, so no need to support integers for now
			// TODO: integers support

			const bool isFormatDouble = r.size == BIT_PER_DOUBLE;
			if (data->mNormals){
				if (isFormatDouble){
					for (std::size_t n=0; n<data->mNumVertices; n++){
						const auto& input = data->mNormals[n];
						void* dest = normals.getRaw(n);

						copy_component_double(r, getAiVec3Component);
						copy_component_double(g, getAiVec3Component);
						copy_component_double(b, getAiVec3Component);
						copy_component_double(a, getAiVec3Component);
					}
				} else {
					for (std::size_t n=0; n<data->mNumVertices; n++){
						const auto& input = data->mNormals[n];
						void* dest = normals.getRaw(n);

						copy_component_float(r, getAiVec3Component);
						copy_component_float(g, getAiVec3Component);
						copy_component_float(b, getAiVec3Component);
						copy_component_float(a, getAiVec3Component);
					}
				}
			} else {
				context.logger->warn("The mesh does not contain normals as required");
				if (isFormatDouble){
					for (std::size_t n=0; n<data->mNumVertices; n++){
						void* dest = normals.getRaw(n);
						
						set_component_double_null(r);
						set_component_double_null(g);
						set_component_double_null(b);
						set_component_double_null(a);
					}
				} else {
					for (std::size_t n=0; n<data->mNumVertices; n++){
						void* dest = normals.getRaw(n);
						
						set_component_float_null(r);
						set_component_float_null(g);
						set_component_float_null(b);
						set_component_float_null(a);
					}
				}
			}
		}
	}

	void loadTangents(AttributeLoadData& loadData){
		const VertexLayout& layout = loadData.layout;
		const std::filesystem::path& filepath = loadData.filepath;
		Context& context = loadData.context;
		MeshData& mesh = loadData.mesh;
		const aiMesh* data = loadData.data;

		auto positionAttributes = layout.getDedicatedAttributes(VertexLayout::TANGENT);

		for (const auto& attribute : positionAttributes){
			auto tangents = mesh.getBinding(attribute.binding).get(static_cast<std::size_t>(attribute.location));

			const Formats::FormatInfo formatInfo = Formats::getInfo(attribute.format);
			const auto& componentSwizzle = attribute.componentMapping;

			// get the components info
			get_component_info(r, RED);
			get_component_info(g, GREEN);
			get_component_info(b, BLUE);
			get_component_info(a, ALPHA);

			// Note that, since these are vertex attributes cannot be smaller than 1 bytes (but can be decomposed),
			// there is no need to handle bit per bit copy

			// For now, the vertex usage are only in float formats, so no need to support integers for now
			// TODO: integers support

			const bool isFormatDouble = r.size == BIT_PER_DOUBLE;
			if (data->mTangents){
				if (isFormatDouble){
					for (std::size_t n=0; n<data->mNumVertices; n++){
						const auto& input = data->mTangents[n];
						void* dest = tangents.getRaw(n);

						copy_component_double(r, getAiVec3Component);
						copy_component_double(g, getAiVec3Component);
						copy_component_double(b, getAiVec3Component);
						copy_component_double(a, getAiVec3Component);
					}
				} else {
					for (std::size_t n=0; n<data->mNumVertices; n++){
						const auto& input = data->mTangents[n];
						void* dest = tangents.getRaw(n);

						copy_component_float(r, getAiVec3Component);
						copy_component_float(g, getAiVec3Component);
						copy_component_float(b, getAiVec3Component);
						copy_component_float(a, getAiVec3Component);
					}
				}
			} else {
				context.logger->warn("The mesh does not contain tangents as required");
				if (isFormatDouble){
					for (std::size_t n=0; n<data->mNumVertices; n++){
						void* dest = tangents.getRaw(n);
						
						set_component_double_null(r);
						set_component_double_null(g);
						set_component_double_null(b);
						set_component_double_null(a);
					}
				} else {
					for (std::size_t n=0; n<data->mNumVertices; n++){
						void* dest = tangents.getRaw(n);
						
						set_component_float_null(r);
						set_component_float_null(g);
						set_component_float_null(b);
						set_component_float_null(a);
					}
				}
			}
		}
	}

	void loadBitangents(AttributeLoadData& loadData){
		const VertexLayout& layout = loadData.layout;
		const std::filesystem::path& filepath = loadData.filepath;
		Context& context = loadData.context;
		MeshData& mesh = loadData.mesh;
		const aiMesh* data = loadData.data;

		auto positionAttributes = layout.getDedicatedAttributes(VertexLayout::BITANGENT);

		for (const auto& attribute : positionAttributes){
			auto bitangents = mesh.getBinding(attribute.binding).get(static_cast<std::size_t>(attribute.location));

			const Formats::FormatInfo formatInfo = Formats::getInfo(attribute.format);
			const auto& componentSwizzle = attribute.componentMapping;

			// get the components info
			get_component_info(r, RED);
			get_component_info(g, GREEN);
			get_component_info(b, BLUE);
			get_component_info(a, ALPHA);

			// Note that, since these are vertex attributes cannot be smaller than 1 bytes (but can be decomposed),
			// there is no need to handle bit per bit copy

			// For now, the vertex usage are only in float formats, so no need to support integers for now
			// TODO: integers support

			const bool isFormatDouble = r.size == BIT_PER_DOUBLE;
			if (data->mBitangents){
				if (isFormatDouble){
					for (std::size_t n=0; n<data->mNumVertices; n++){
						const auto& input = data->mBitangents[n];
						void* dest = bitangents.getRaw(n);

						copy_component_double(r, getAiVec3Component);
						copy_component_double(g, getAiVec3Component);
						copy_component_double(b, getAiVec3Component);
						copy_component_double(a, getAiVec3Component);
					}
				} else {
					for (std::size_t n=0; n<data->mNumVertices; n++){
						const auto& input = data->mBitangents[n];
						void* dest = bitangents.getRaw(n);

						copy_component_float(r, getAiVec3Component);
						copy_component_float(g, getAiVec3Component);
						copy_component_float(b, getAiVec3Component);
						copy_component_float(a, getAiVec3Component);
					}
				}
			} else {
				context.logger->warn("The mesh does not contain bitangents as required");
				if (isFormatDouble){
					for (std::size_t n=0; n<data->mNumVertices; n++){
						void* dest = bitangents.getRaw(n);
						
						set_component_double_null(r);
						set_component_double_null(g);
						set_component_double_null(b);
						set_component_double_null(a);
					}
				} else {
					for (std::size_t n=0; n<data->mNumVertices; n++){
						void* dest = bitangents.getRaw(n);
						
						set_component_float_null(r);
						set_component_float_null(g);
						set_component_float_null(b);
						set_component_float_null(a);
					}
				}
			}
		}
	}


	std::vector<MeshData> ModelLoader::loadMeshes(const std::filesystem::path& filepath, const ModelLayoutConfig& config){
		if (!_context){
			spdlog::warn("Attempt to load a model using a non prepared model loader");
			throw std::runtime_error("The model loader has not been prepared");
		}

		_context->logger->info("Loading meshes \"{}\"...", filepath.string());


		Assimp::Importer importer;
		uint flags = aiProcess_Triangulate | aiProcess_SortByPType | aiProcess_OptimizeGraph | aiProcess_OptimizeMeshes | aiProcess_GenSmoothNormals;
		
		_context->logger->trace("Reading model file \"{}\"...", filepath.string());
		const aiScene* scene = importer.ReadFile(filepath.string(), flags);
		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode){
			_context->logger->error("Failed to load model scene \"{}\" : reason  : {}", filepath.string(), importer.GetErrorString());
			throw std::runtime_error("Failed to load model scene");
		}

		std::vector<MeshData> meshes(scene->mNumMeshes);
		_context->logger->trace("Model file loaded successfully \"{}\", found {} meshes", filepath.string(), scene->mNumMeshes);;
		


		// Loading each meshes
		for (std::size_t i=0; i<scene->mNumMeshes; i++){
			const auto& data = scene->mMeshes[i];
			auto& mesh = meshes[i];

			// set the mesh primtive type
			mesh.setPrimitiveTopology(getPrimitiveTopology(data->mPrimitiveTypes));
			
			// Filter out layout
			const ModelLayoutConfig::MeshProfile* profile = config.find(getMeshLayoutUsage(data));
			if (profile == nullptr){
				_context->logger->error("Could not load model \"{}\", the model layout config does not contains a suitable profile for this mesh");
				throw std::runtime_error("Could not find suitable mesh profile");
			}

			const VertexLayout& layout = profile->getLayout();

			// prepare the mesh for data collection
			mesh.prepare(*_context)
				.setLayout(layout)
				.allocate(data->mNumVertices);


			// copy positions 
			spdlog::info("loading model");

			AttributeLoadData loadData{
				layout,
				filepath,
				*_context,
				mesh,
				data
			};

			loadPositions(loadData);
			loadNormals(loadData);
			loadColors(loadData);
			loadUVs(loadData);
			loadTangents(loadData);
			loadBitangents(loadData);

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
}