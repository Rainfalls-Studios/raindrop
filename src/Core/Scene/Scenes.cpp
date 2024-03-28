#include <Raindrop/Core/Scene/Scenes.hpp>
#include <Raindrop/Context.hpp>

#include <spdlog/spdlog.h>

namespace Raindrop::Core::Scene{
	Scenes::Scenes(Context& context) : _context{context}{
		spdlog::info("Constructing scene manager ...");
	}

	Scenes::~Scenes(){
		spdlog::info("Destroying scene manager ... ({} scenes)", _scenes.size() - _freeIDs.size());
		_scenes.clear();
	}

	SceneID Scenes::registerScene(const std::shared_ptr<Scene>& scene){
		spdlog::info("registring scene into the scene manager ...");
		if (_freeIDs.empty()){
			_scenes.push_back(scene);
			return static_cast<SceneID>(_scenes.size() - 1);
		}

		SceneID ID = _freeIDs.front();
		_freeIDs.pop();

		_scenes[static_cast<std::size_t>(ID)] = scene;
		return ID;
	}

	void Scenes::unregisterScene(const SceneID& ID){
		spdlog::info("unregistering scene from the scene manager ... (ID : {})", ID);
		assert(ID < _scenes.size() && "Invalid scene ID");
		assert(_scenes[ID] != nullptr && "The scene has already been unregistred");

		_scenes[ID] = nullptr;
		_freeIDs.push(ID);
	}

	Scene& Scenes::get(const SceneID& ID){
		assert(ID < _scenes.size() && "Invalid scene ID");
		assert(_scenes[ID] != nullptr && "Attempting to access a non registred scene");
		return *_scenes[ID];
	}
}