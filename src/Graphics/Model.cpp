#include <Raindrop/Graphics/Model.hpp>
#include <Raindrop/Graphics/Context.hpp>
#include <Raindrop/Graphics/Mesh.hpp>
#include <Raindrop/Context.hpp>

namespace Raindrop::Graphics{
	std::shared_ptr<Model> Model::create(Raindrop::Context& context){
		return context.registry.emplace<Model>();
	}

	Model::Model() noexcept : 
		_context{nullptr},
		_meshes{}
	{}

	Model::~Model(){
		release();
	}

	Model::Model(Model&& other) : 
		_context{nullptr},
		_meshes{}
	{
		swap(*this, other);
	}

	Model& Model::operator=(Model&& other){
		swap(*this, other);
		return *this;
	}

	void swap(Model& A, Model& B){
		std::swap(A._context, B._context);
		std::swap(A._meshes, B._meshes);
	}
	
	Model& Model::prepare(Context& context){
		_context = &context;
		return *this;
	}

	void Model::initialize(){
		if (!_context){
			spdlog::warn("Attempt to initialize a non prepared model");
			throw std::runtime_error("The model must be prepared before initialization");
		}
	}

	void Model::release(){
		_meshes.clear();
		_context = nullptr;
	}

	Model& Model::setMeshes(std::vector<Mesh>&& meshes){
		_meshes = std::move(meshes);
		return *this;
	}

	const std::vector<Mesh>& Model::getMeshes() const noexcept{
		return _meshes;
	}
}