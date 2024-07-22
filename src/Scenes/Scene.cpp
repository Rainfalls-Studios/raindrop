#include <Raindrop/Scenes/Scene.hpp>
#include <Raindrop/Scenes/Context.hpp>
#include <Raindrop/Scenes/Property.hpp>

namespace Raindrop::Scenes{

	Scene::Scene() noexcept : 
		_context{nullptr},
		_properties{}
	{}

	Scene::~Scene(){
		release();
	}

	Scene::Scene(Scene&& other) : 
		_context{nullptr},
		_properties{}
	{
		Scene::swap(*this, other);
	}
	
	Scene& Scene::operator=(Scene&& other){
		Scene::swap(*this, other);
		return *this;
	}

	void Scene::swap(Scene& A, Scene& B){
		// std::swap(static_cast<Registry&>(A), static_cast<Registry&>(B));
		// static_cast<Registry&>(A).swap()
		static_cast<Registry&>(A).swap(static_cast<Registry&>(B));
		std::swap(A._context, B._context);
		std::swap(A._properties, B._properties);
	}

	void Scene::prepare(Context& context){
		_context = &context;
	}

	void Scene::initialize(){

	}

	void Scene::release(){
		_properties.clear();
		Registry::clear();
		_context = nullptr;
	}

	Entity Scene::create(){
		EntityID id = Registry::create();
		return Entity(*this, id);
	}

	Entity Scene::create(const EntityID& hint){
		EntityID id = Registry::create(hint);
		return Entity(*this, id);
	}

	void Scene::destroy(const Entity& entity){
		Registry::destroy(entity.getID());
	}

	Registry& Scene::getRegistry() noexcept{
		return *this;
	}

	const Registry& Scene::getRegistry() const noexcept{
		return *this;
	}
}