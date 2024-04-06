#include <Raindrop/Core/Scenes/SceneRegistry.hpp>
#include <Raindrop/Core/Scenes/Scene.hpp>
#include <Raindrop/Context.hpp>

#include <spdlog/spdlog.h>

namespace Raindrop::Core::Scenes{
	SceneRegistry::SceneRegistry(Context& context) : _context{context}{
		spdlog::info("Constructing scene registry ...");
	}

	SceneRegistry::~SceneRegistry(){
		spdlog::info("Destroying scene registry ... ({} scene(s))", Registry::usedSize());
	}

	SceneID SceneRegistry::createScene(){
		return registerScene(std::make_unique<Scene>());
	}

	SceneID SceneRegistry::registerScene(std::unique_ptr<Scene>&& scene){
		spdlog::info("Registring scene into the scene registry ...");
		return Registry::registerHandle(std::move(scene));
	}

	void SceneRegistry::unregisterScene(const SceneID& ID){
		spdlog::info("Unregistering scene from the scene registry ... (ID : {})", ID);
		Registry::destroy(ID);
	}

	Scene& SceneRegistry::get(const SceneID& ID){
		auto& ptr = Registry::get(ID);
		if (ptr == nullptr){
			throw std::runtime_error("Invalid Scene ID");
		}
		return *ptr;
	}
}