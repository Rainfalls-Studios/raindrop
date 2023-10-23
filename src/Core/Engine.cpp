#include <Raindrop/Core/Engine.hpp>
#include <Raindrop/Core/Context.hpp>

namespace Raindrop::Core{
	Engine::Engine(){
		_context = std::make_unique<Context>();
		_context->logger.info("Initializing Core engine...");
		_context->logger.info("Core initialized without any critical error");
	}

	Engine::~Engine(){
		_context->logger.info("Terminating Core engine...");
		_context->logger.info("Core terminated without any critical error");
		_context.reset();
	}
	
	Event::EventManager& Engine::eventManager(){
		return _context->eventManager;
	}

	const Event::EventManager& Engine::eventManager() const{
		return _context->eventManager;
	}
	
	void Engine::triggerEvent(const std::string& event){
		_context->eventManager.trigger(event);
	}

	void Engine::subscribeEvent(const std::string& event, Event::Callback callback){
		_context->eventManager.subscribe(event, callback);
	}
	
	Event::KeyEvents& Engine::keyEvents(){
		return _context->eventManager.keyEvents();
	}

	Event::MouseEvents& Engine::mouseEvents(){
		return _context->eventManager.mouseEvents();
	}
	
	const Event::KeyEvents& Engine::keyEvents() const{
		return _context->eventManager.keyEvents();
	}

	const Event::MouseEvents& Engine::mouseEvents() const{
		return _context->eventManager.mouseEvents();
	}

	Asset::AssetManager& Engine::assetManager(){
		return _context->assetManager;
	}

	const Asset::AssetManager& Engine::assetManager() const{
		return _context->assetManager;
	}

	std::weak_ptr<Asset::Asset> Engine::loadOrGet(const std::filesystem::path& path){
		return _context->assetManager.loadOrGet(path);
	}

	void Engine::registerFactory(const std::shared_ptr<Asset::AssetFactory>& factory, std::size_t typeID){
		return _context->assetManager.registerFactory(factory, typeID);
	}

	void Engine::removeFactory(std::size_t typeID){
		return _context->assetManager.removeFactory(typeID);
	}

	Scene::SceneManager& Engine::sceneManager(){
		return _context->sceneManager;
	}

	const Scene::SceneManager& Engine::sceneManager() const{
		return _context->sceneManager;
	}
	
	std::shared_ptr<Scene::Scene> Engine::createScene(const std::string& name){
		return _context->sceneManager.create(name);
	}

	std::shared_ptr<Scene::Scene> Engine::getOrCreateScene(const std::string& name){
		return _context->sceneManager.getOrCreate(name);
	}

	std::shared_ptr<Scene::Scene> Engine::getScene(const std::string& name){
		return _context->sceneManager.get(name);
	}

	const std::shared_ptr<Scene::Scene> Engine::getScene(const std::string& name) const{
		return _context->sceneManager.get(name);
	}

	void Engine::removeScene(const std::string& name){
		return _context->sceneManager.remove(name);
	}

	void Engine::removeScene(const std::shared_ptr<Scene::Scene>& scene){
		return _context->sceneManager.remove(scene);
	}

	bool Engine::existsScene(const std::string& name) const{
		return _context->sceneManager.exists(name);
	}

	Registry::Registry& Engine::registry(){
		return _context->registry;
	}

	const Registry::Registry& Engine::registry() const{
		return _context->registry;
	}

	Asset::Context& Engine::assetContext(){
		return *_context->assetManager._context;
	}
}