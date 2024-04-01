#include <Raindrop/Core/Scenes/Registry.hpp>
#include <Raindrop/Core/Scenes/Scene.hpp>
#include <Raindrop/Context.hpp>

#include <spdlog/spdlog.h>

namespace Raindrop::Core::Scenes{
	Registry::Registry(Context& context) : _context{context}{
		spdlog::info("Constructing scene manager ...");
	}

	Registry::~Registry(){
		spdlog::info("Destroying scene manager ... ({} Registry)", _scenes.size() - _freeIDs.size());
		_scenes.clear();
	}

	SceneID Registry::createScene(){
		return registerScene(std::make_unique<Scene>());
	}

	SceneID Registry::registerScene(std::unique_ptr<Scene> scene){
		spdlog::info("Registring scene into the scene manager ...");
		if (_freeIDs.empty()){
			_scenes.push_back(std::move(scene));
			return static_cast<SceneID>(_scenes.size() - 1);
		}

		SceneID ID = _freeIDs.front();
		_freeIDs.pop();

		_scenes[static_cast<std::size_t>(ID)] = std::move(scene);
		return ID;
	}

	void Registry::unregisterScene(const SceneID& ID){
		spdlog::info("Unregistering scene from the scene manager ... (ID : {})", ID);
		assert(ID < _scenes.size() && "Invalid scene ID");
		assert(_scenes[ID] != nullptr && "The scene has already been unregistred");

		_scenes[ID] = nullptr;
		_freeIDs.push(ID);
	}

	Scene& Registry::get(const SceneID& ID){
		assert(ID < _scenes.size() && "Invalid scene ID");
		assert(_scenes[ID] != nullptr && "Attempting to access a non registred scene");
		return *_scenes[ID];
	}
}