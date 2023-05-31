#include <Raindrop/Core/Scene/Scene.hpp>
#include <Raindrop/Core/Scene/Entity.hpp>
#include <Raindrop/Core/Registry/Registry.hpp>

namespace Raindrop::Core::Scene{
	Scene::Scene(Registry::Registry& manager) : _registry{manager.get(std::to_string(reinterpret_cast<uintptr_t>(this)))}{
		el::Logger* customLogger = el::Loggers::getLogger("Engine.Core.Scene");
		customLogger->configurations()->set(el::Level::Global, el::ConfigurationType::Format, "%datetime %level [%logger]: %msg");
		CLOG(INFO, "Engine.Core.Scene") << "New Scene created";
		_root = std::make_unique<Entity>();
	}

	Scene::~Scene(){
		CLOG(INFO, "Engine.Core.Scene") << "Scene destroyed";
	}

	Entity& Scene::root(){
		return *_root.get();
	}

	void drawChilds(Entity& e){
		printf("-%s:\n", e.tag.c_str());
		for (auto &c : e){
			drawChilds(c);
		}
	}

	void Scene::tree(){
		drawChilds(root());
	}

	Registry::Registry& Scene::registry() const{
		return _registry;
	}
}