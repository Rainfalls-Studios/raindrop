#include <Raindrop/Raindrop.hpp>
#include <Raindrop/Context.hpp>
#include <spdlog/spdlog.h>
#include <Raindrop/Core/Event/Manager.hpp>

#include <Raindrop/Modules/Loader.hpp>

namespace Raindrop{
	Version::Version() noexcept :
		variant{0},
		major{0},
		minor{0},
		patch{0}
	{}

	Version::Version(const Version& other) noexcept :
		variant{other.variant},
		major{other.major},
		minor{other.minor},
		patch{other.patch}
	{}

	Version::Version(std::size_t variant, std::size_t major, std::size_t minor, std::size_t patch) noexcept : 
		variant{variant},
		major{major},
		minor{minor},
		patch{patch}
	{}

	Version& Version::operator=(const Version& other) noexcept{
		variant = other.variant;
		major = other.major;
		minor = other.minor;
		patch = other.patch;
		return *this;
	}


	Raindrop::Raindrop(){
		spdlog::info("Constructing Raindrop engine ...");
		_context = new Context();

		createAssetLoader<Modules::Loader>("Module", *_context);
		registerEvent("OnTick");
	}

	void Raindrop::run(){
		spdlog::info("Starting to run...");
		_context->running = true;
		
		while (_context->running){
			triggerEvent("OnTick");
			render();
			events();
			updateCameraPosition();
		}
	}

	Raindrop::~Raindrop(){
		spdlog::info("Destroying Raindrop engine ...");
		
		unregisterAssetLoader("Module");
		delete _context;
	}

	void Raindrop::render(){
		_context->renderer.render();
	}

	void Raindrop::events(){
		_context->renderer.events();
	}

	void Raindrop::updateCameraPosition(){
		
	}

	SceneWrapper Raindrop::createScene(){
		return SceneWrapper(*_context, _context->scenes.createScene());
	}

	SceneWrapper Raindrop::registerScene(std::unique_ptr<Scene> scene){
		return SceneWrapper(*_context, _context->scenes.registerScene(std::move(scene)));
	}

	void Raindrop::unregisterScene(const SceneWrapper& wrapper){
		return _context->scenes.unregisterScene(wrapper.ID());
	}

	SceneWrapper Raindrop::getScene(const SceneID& ID){
		return SceneWrapper(*_context, ID);
	}

	AssetManager& Raindrop::assetManager(){
		return _context->assetManager;
	}

	EventManager& Raindrop::eventManager(){
		return _context->eventManager;
	}

	Graphics::Renderer& Raindrop::renderer(){
		return _context->renderer;
	}

	void Raindrop::quit(){
		_context->running = false;
	}

	std::shared_ptr<Asset> Raindrop::getAsset(const std::string& type, const Path& path){
		return _context->assetManager.get(type, path);
	}

	void Raindrop::registerAssetLoader(const std::string& type, const std::shared_ptr<AssetLoader>& loader){
		return _context->assetManager.registerLoader(type, loader);
	}

	void Raindrop::unregisterAssetLoader(const std::string& type){
		_context->assetManager.unregisterType(type);
	}

	std::shared_ptr<AssetLoader> Raindrop::getAssetLoader(const std::string& type){
		return _context->assetManager.findLoader(type);
	}

	std::shared_ptr<Module> Raindrop::getModule(const std::string& alias){
		auto moduleLoader = getAssetLoader<Modules::Loader>("Module");
		assert(moduleLoader);

		return moduleLoader->get(alias);
	}

	std::shared_ptr<Module> Raindrop::loadModule(const Path& path){
		return getAsset<Module>("Module", path);
	}
}