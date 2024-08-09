#include <Raindrop/Engine.hpp>
#include <Raindrop/Graphics/common.hpp>

#define create_graphics_interface(type) std::shared_ptr<Graphics::type> Engine::createGraphics##type(){ std::shared_ptr<Graphics::type> instance = getUUIDRegistry().emplace<Graphics::type>(); instance->prepare(getGraphicsContext()); return std::move(instance);}

namespace Raindrop{
	Engine::Engine() :
		_context{nullptr},
		_graphics{nullptr}
	{}

	Engine::~Engine(){
		release();
	}

	void Engine::initialize(InitFlags flags){
		initializeCore();

		if (flags & INIT_EVENTS){
			initializeEvents();
		}

		if (flags & INIT_GRAPHICS){
			initializeGraphics();
		}

		if (flags & INIT_SCENES){
			initializeScenes();
		}
	}

	void Engine::release(){
		_scenes.reset();
		_graphics.reset();
		_events.reset();
		_context.reset();
	}

	void Engine::initializeCore(){
		_context = std::make_unique<Context>();
		// _context->initialize()
	}

	void Engine::initializeGraphics(){
		_graphics = std::make_unique<Graphics::Context>();
		_graphics->initialize(getEventsContext());

		// _graphics->core.createLogger();
		// _graphics->core.instance.initialize(_graphics->core);
	}

	void Engine::initializeEvents(){
		_events = std::make_unique<Events::Context>();
		_events->initialize();
	}	
	
	void Engine::initializeScenes(){
		_scenes = std::make_unique<Scenes::Context>();
		_scenes->initialize(*_context);
	}


	// ======================= CORE ==============================

	Context& Engine::getContext(){
		if (!_context){
			throw std::runtime_error("Core has not been initialized");
		}
		return *_context;
	}

	const Context& Engine::getContext() const{
		if (!_context){
			throw std::runtime_error("Core has not been initialized");
		}
		return *_context;
	}

	UUIDRegistry& Engine::getUUIDRegistry(){
		return getContext().registry;
	}

	// ==================== GRAPHICS ============================

	Graphics::Context& Engine::getGraphicsContext(){
		if (!_graphics){
			throw std::runtime_error("Graphics context has not been initialized");
		}
		return *_graphics;
	}

	const Graphics::Context& Engine::getGraphicsContext() const{
		if (!_graphics){
			throw std::runtime_error("Graphics context has not been initialized");
		}
		return *_graphics;
	}


	Graphics::Core::Instance& Engine::getGraphicsInstance(){
		return getGraphicsContext().core.instance;
	}

	Graphics::Core::PhysicalDevice& Engine::getGraphicsPhysicalDevice(){
		return getGraphicsContext().core.physicalDevice;
	}

	Graphics::Core::Device& Engine::getGraphicsDevice(){
		return getGraphicsContext().core.device;
	}

	Graphics::Window::Window& Engine::getGraphicsWindow(){
		return getGraphicsContext().window.window;
	}

	Graphics::Window::Surface& Engine::getGraphicsWindowSurface(){
		return getGraphicsContext().window.surface;
	}

	Graphics::Window::Swapchain& Engine::getGraphicsWindowSwapchain(){
		return getGraphicsContext().window.swapchain;
	}

	Graphics::Formats& Engine::getGraphicsFormats(){
		return getGraphicsContext().formats;
	}

	Graphics::ModelLoader& Engine::getGraphicsModelLoader(){
		return getGraphicsContext().modelLoader;
	}

	create_graphics_interface(RenderPass);
	create_graphics_interface(Buffer);
	create_graphics_interface(DescriptorSetLayout);
	create_graphics_interface(GraphicsPipeline);
	create_graphics_interface(ShaderModule);
	create_graphics_interface(Memory);
	create_graphics_interface(Image);
	create_graphics_interface(Sampler);
	create_graphics_interface(DescriptorPool);
	create_graphics_interface(MeshData);
	
	Graphics::ModelLayoutConfig Engine::createGraphicsModelLayoutConfig(){
		return Graphics::ModelLayoutConfig();
	}

	Graphics::VertexLayout Engine::createGraphicsVertexLayout(){
		return Graphics::VertexLayout();
	}

	// ==================== EVENTS ============================

	Events::Context& Engine::getEventsContext(){
		if (!_events){
			throw std::runtime_error("Events context has not been initialized");
		}
		return *_events;
	}

	const Events::Context& Engine::getEventsContext() const{
		if (!_events){
			throw std::runtime_error("Events context has not been initialized");
		}
		return *_events;
	}


	// ==================== SCENES ============================

	Scenes::Context& Engine::getScenesContext(){
		if (!_scenes){
			throw std::runtime_error("Scenes context has not been initialized");
		}
		return *_scenes;
	}

	const Scenes::Context& Engine::getScenesContext() const{
		if (!_scenes){
			throw std::runtime_error("Scenes context has not been initialized");
		}
		return *_scenes;
	}

	std::shared_ptr<Scenes::Scene> Engine::createScene(){
		std::shared_ptr<Scenes::Scene> scene = _context->registry.emplace<Scenes::Scene>();
		scene->prepare(getScenesContext());
		return scene;
	}
}