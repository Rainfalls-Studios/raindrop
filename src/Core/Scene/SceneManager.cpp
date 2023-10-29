#include <Raindrop/Core/Scene/SceneManager.hpp>
#include <Raindrop/Core/Scene/Context.hpp>
#include <Raindrop/Core/Scene/Scene.hpp>

namespace Raindrop::Core::Scene{
	SceneManager::SceneManager(Core::Context& core){
		_context = std::make_unique<Context>(core);

		_context->logger().info("Initializing Scene manager...");
		_context->logger().info("Scene manager initialized without any critical error");
	}

	SceneManager::~SceneManager(){
		_context->logger().info("Terminating Scene manager...");
		_context->logger().info("Scene manager terminated without any critical error");
		_context.reset();
	}

	std::shared_ptr<Scene> SceneManager::create(const std::string& name){
		_context->logger().info("Creating a new scene...");

		std::shared_ptr<Scene> scene;
		try{
			scene = std::make_shared<Scene>(*_context);
		} catch (const std::exception& e){
			_context->logger().error("Failed to create scene : %s", e.what());
			throw std::runtime_error("Failed to create a scene");
		}

		_context->logger().info("New scene create with success !");
		_nameToSceneMap[name] = scene;
		return scene;
	}

	std::shared_ptr<Scene> SceneManager::getOrCreate(const std::string& name){
		if (exists(name)) return get(name);
		return create(name);
	}

	std::shared_ptr<Scene> SceneManager::get(const std::string& name){
		return _nameToSceneMap[name];
	}

	bool SceneManager::exists(const std::string& name) const{
		return _nameToSceneMap.find(name) != _nameToSceneMap.end();
	}

	void SceneManager::remove(const std::string& name){
		_context->logger().info("Removing scene \"%s\" from scene manager", name.c_str());
		_nameToSceneMap.erase(name);
	}

	void SceneManager::remove(const std::shared_ptr<Scene>& scene){
		remove(scene->name());
	}
	
	const std::shared_ptr<Scene> SceneManager::get(const std::string& name) const{
		auto it = _nameToSceneMap.find(name);
		if (it == _nameToSceneMap.end()){
			_context->logger().warn("Cannot find scene \"%s\"", name.c_str());
			throw std::runtime_error("Failed to find scene");
		}
		return it->second;
	}
}